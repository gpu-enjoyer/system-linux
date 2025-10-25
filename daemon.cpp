
#include <csignal>          // OS' signals
#include <syslog.h>         // System log journal

#include <yaml-cpp/yaml.h>  // Read config.yaml
#include <iostream>         // UI, debug

#include <filesystem>       // backup(fs::path source, target)

// #include <thread>        // std::this_thread::sleep_for
// #include <chrono>        // wake_up(when_the_sun_is_shining)

namespace fs = std::filesystem;


void dyingWish(
    int sign)
{
    syslog(LOG_NOTICE, " === backup-daemon stopped === "); 
    closelog();
    exit(sign);
}

void readConfig(
    std::string& source,
    std::string& target,
    int&         freq)
{
    std::string freqStr;

    try {
        YAML::Node config = YAML::LoadFile("config.yaml");
        source  = config["source"].as<std::string>();
        target  = config["target"].as<std::string>();
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

    if      (freqStr == "min")   freq = 1;
    else if (freqStr == "hour")  freq = 60;
    else if (freqStr == "day")   freq = 1440;
    else if (freqStr == "week")  freq = 10080;
    else {
        syslog(LOG_ERR, "ERROR: config.yaml: freq = \"%s\" is not valid", freqStr.c_str());
        dyingWish(-1);
    }
}


int main()
{
    std::string  source, target;
    int          freq;

    // register dyingWish() as exit-func()
    signal(SIGTERM, dyingWish);

    openlog("backup-daemon", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, " === backup-daemon started === ");

    readConfig(source, target, freq);

    std::cout
        << "source = " << source << '\n'
        << "target = " << target << '\n'
        << "freq   = " << freq   << '\n';
    
    // ?  backup: std::fs vs memcpy()

    // when (theTimeHasCome(freq)) {
    //   wake_up();
    //   backup(source, target);
    //   sleep(freq);
    // }

    return 0;
}
