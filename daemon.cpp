
#include <csignal>  // OS' signals
#include <syslog.h> // System log journal

#include <yaml-cpp/yaml.h> // Read config.yaml

#include <filesystem> // backup(source, target)
namespace fs = std::filesystem;

#include <thread>    // std::this_thread::sleep_for
#include <chrono>    // Control sleep Time
#include <iostream>  // UI
#include <iomanip>   // std::put_time
#include <sstream>   // std::stringstream

void dyingWish(int sign);

class Daemon {

    fs::path  source;
    fs::path  target;
    int       freq;

    void init()
    {
        signal(SIGTERM, dyingWish); // set dyingWish() as a termination function
        openlog("backup-daemon", LOG_PID, LOG_DAEMON);
        syslog(LOG_NOTICE, " === backup-daemon started === ");
    }

    // configPath -> str -> check -> Daemon fields
    void readConfig(std::string configPath)
    {
        fs::path sourcePath;
        fs::path targetPath;
        std::string freqStr;

        try {
            YAML::Node config = YAML::LoadFile("config.yaml");
            source = config["source"].as<std::string>();
            target = config["target"].as<std::string>();
            freqStr = config["freq"].as<std::string>();
        }
        catch (const YAML::BadFile& e) {
            syslog(LOG_ERR, "ERROR: config.yaml could not be opened");
            dyingWish(-1);
        }
        catch (const YAML::ParserException& e) {
            syslog(LOG_ERR, "ERROR: config.yaml parse error");
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
            " === backup-daemon backup() successfull === "
        );

        syslog(
            LOG_NOTICE,
            "[ %s ] -> [ %s ]",
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

void dyingWish(int sign)
{
    syslog(LOG_NOTICE, " === backup-daemon stopped === ");
    closelog();
    exit(sign);
}

int main()
{
    Daemon D("config.yaml");
    // ...
    return 0;
}
