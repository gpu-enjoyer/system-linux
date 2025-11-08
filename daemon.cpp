
//! Возможно, написанное ниже полезно и правильно,
//!  но это точно не то, с чего надо начать.
//!   Многие возможности инкапсулированы в системных библиотеках.

//  push:        "in-progress" branch
//  checkout -b: "tutorial" branch

#include <csignal>
// OS' signals

#include <syslog.h>
// System log journal

#include <yaml-cpp/yaml.h>
// Read config.yaml

#include <filesystem>
namespace fs = std::filesystem;
// backup(fs::path source, fs::path target)


void dyingWish(int sign)
{
    syslog(LOG_NOTICE, " === backup-daemon stopped === "); 
    closelog();
    exit(sign);
}


enum class freqEnum: uint8_t
{
    min,  // 0
    hour, // 1
    day,  // 2
    week  // 3
};

freqEnum fromStr(const std::string& freqStr)
{
    if (freqStr == "min")  return freqEnum::min;
    if (freqStr == "hour") return freqEnum::hour;
    if (freqStr == "day")  return freqEnum::day;
    if (freqStr == "week") return freqEnum::week;
    throw std::invalid_argument("Invalid freq value: " + freqStr);
}

class Daemon {

    fs::path source;
    fs::path target;
    freqEnum frequency;

    void init()
    {
        signal(SIGTERM, dyingWish);
        // (termination_signal, termination_function)

        openlog("backup-daemon", LOG_PID, LOG_DAEMON);
        // (proc_name, proc_id, proc_type)

        syslog(LOG_NOTICE, "started!");
    }

    void readConfig(std::string configPath)
    {
        try
        {
            // todo: install.sh -> system path to config
            YAML::Node config = YAML::LoadFile("config.yaml");

            source    = config["source"].as<std::string>();
            target    = config["target"].as<std::string>();
            frequency = fromStr( config["frequency"].as<std::string>() );
        }
        catch (const YAML::BadFile& e)
        {
            syslog(LOG_ERR, "Bad config.yaml (path: %s)", configPath.c_str());
            dyingWish(-1);
        }
        catch (const YAML::ParserException& e)
        {
            syslog(LOG_ERR, "Parse error: config.yaml (path: %s)", configPath.c_str());
            dyingWish(-1);
        }

        if      (freqStr == "min")  freq = 1;
        else if (freqStr == "hour") freq = 60;
        else if (freqStr == "day")  freq = 1440;
        else if (freqStr == "week") freq = 10080;
        else {
            syslog(LOG_ERR, "ERROR: config.yaml: freq = \"%s\" is not valid", freqStr.c_str());
            dyingWish(-1);
        }

        sourcePath = fs::path(source);
        targetPath = fs::path(target);

        if (!fs::exists(sourcePath))
        {
            syslog(LOG_ERR, "ERROR: config.yaml: source = %s does not exist", source.c_str());
            dyingWish(-1);
        }

        sourcePath = fs::absolute(sourcePath);

        if (!fs::exists(targetPath))
        {
            syslog(LOG_ERR, "ERROR: config.yaml: target = %s does not exist", target.c_str());
            dyingWish(-1);
        }

        targetPath = fs::absolute(targetPath);

        if (!fs::is_directory(targetPath)) {
            syslog(LOG_ERR, "ERROR: config.yaml: target = %s is not a directory", target.c_str());
            dyingWish(-1);
        }

        if (!fs::is_directory(sourcePath)) {
            syslog(LOG_ERR, "ERROR: config.yaml: source = %s is not a directory", source.c_str());
            dyingWish(-1);
        }

        std::vector<fs::path> forbidden = {"/bin", "/boot", "/dev", "/etc", "/lib", "/proc", "/root", "/sbin", "/sys", "/usr"};

        std::string src_str = sourcePath.string();
        std::string tgt_str = targetPath.string();

        for (const auto& f : forbidden)
        {
            std::string forb_str = f.string();
            if (src_str.find(forb_str) == 0 || tgt_str.find(forb_str) == 0)
            {
                syslog(LOG_ERR, "ERROR: config.yaml: path points to forbidden system directory: %s", forb_str.c_str());
                dyingWish(-1);
            }
        }

        this->source = sourcePath;
        this->target = targetPath;
    }

    // backupDir = target/backup_DD-MM-YYYY_HH:MM
    void backup(
        fs::path sourcePath,
        fs::path targetPath)
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm bt = *std::localtime(&in_time_t);
        std::stringstream ss;
        ss << "backup_" << std::put_time(&bt, "%d-%m-%Y_%H:%M");
        std::string backupDirName = ss.str();
        fs::path backupDir = targetPath / backupDirName;

        try {
            fs::create_directory(backupDir);
            fs::path dest = backupDir / sourcePath.filename();
            fs::copy(sourcePath, dest, fs::copy_options::recursive);
        }
        catch (const fs::filesystem_error& e) {
            syslog(LOG_ERR, "ERROR: backup failed: %s", e.what());
            return;
        }

        syslog(
            LOG_NOTICE,
            "%s  ->  %s\
            backup DONE",
            sourcePath.c_str(),
            (backupDir / sourcePath.filename()).c_str()
        );
    }

public:

    Daemon(std::string configPath)
    {
        init();
        while (true)
        {
            readConfig(configPath);
            backup(source, target);
            // Спать в течение времени
            std::this_thread::sleep_for(std::chrono::minutes(freq));
        }
    }
};


int main()
{
    Daemon D("config.yaml");
    // ...
    return 0;
}
