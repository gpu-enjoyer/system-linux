
#include <csignal>  // OS' signals
#include <syslog.h> // System log journal

#include <yaml-cpp/yaml.h> // Read config.yaml

#include <filesystem> // backup(source, target)
namespace fs = std::filesystem;

// #include <thread>           // std::this_thread::sleep_for
// #include <chrono>           // Control sleep Time
// #include <iostream>         // UI


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

    // todo: configPath -> str -> check -> Daemon fields
    void readConfig(std::string configPath)
    {
        fs::path     sourcePath;
        fs::path     targetPath;
        std::string  freqStr;

        try
        {
            YAML::Node config = YAML::LoadFile("config.yaml");
            source  = config["source"].as<std::string>();
            target  = config["target"].as<std::string>();
            freqStr = config["freq"].as<std::string>();
        }
        catch (const YAML::BadFile& e)
        {
            syslog(LOG_ERR, "ERROR: config.yaml could not be opened");
            dyingWish(-1);
        }
        catch (const YAML::ParserException& e)
        {
            syslog(LOG_ERR, "ERROR: config.yaml parse error");
            dyingWish(-1);
        }

        if      (freqStr == "min")   freq = 1;
        else if (freqStr == "hour")  freq = 60;
        else if (freqStr == "day")   freq = 1440;
        else if (freqStr == "week")  freq = 10080;
        else
        {
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

        // проверить, что пути не указывают на системные файлы, иначе: ошибка
        // проверить, что targetPath - папка, иначе: ошибка
    }


    void backup(
        fs::path sourcePath,
        fs::path targetPath)
    {
        // создать папку backupDir = target/backup_DD-MM-YYYY_HH:MM
        // копировать source -> backupDir

        syslog(
            LOG_NOTICE,
            " === backup-daemon backup() successfull === "
        );

        syslog(
            LOG_NOTICE,
            "[ %s ] -> [ %s ]",
            sourcePath.c_str(),
            targetPath.c_str()
        );
    }

public:

    Daemon(std::string configPath)
    {
        init();
        while (true)
        {
            readConfig(configPath);
            // backup();
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
    return 0;
}
