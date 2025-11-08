
#include "daemon.hpp"
#define err(msg) std::runtime_error(msg)


bool Config::getFrequency(
    const std::string freqStr)
{
    if      (freqStr == "min")  frequency = 1;
    else if (freqStr == "hour") frequency = 60;
    else if (freqStr == "day")  frequency = 1440;
    else if (freqStr == "week") frequency = 10080;
    else    return false;
    return true;
}

Config::Config(
    fs::path configPath)
{
    if (!fs::exists(configPath)) throw err("config.yaml does not exist");
    configPath = fs::canonical(configPath);

    std::string freqStr;

    try {
        YAML::Node configYaml = YAML::LoadFile(configPath);
        source  = configYaml["source"].as<std::string>();
        target  = configYaml["target"].as<std::string>();
        freqStr = configYaml["frequency"].as<std::string>();
    }
    catch (const YAML::BadFile& e) {
        throw err("config.yaml: bad file: " + configPath.string());
    }
    catch (const YAML::ParserException& e) {
        throw err("config.yaml: parser exception: " + configPath.string());
    }

    if (!getFrequency(freqStr)) throw err("config.yaml: invalid \"frequency\" value");

    if (!fs::exists(source))
        throw err("config.yaml: \"source\" does not exist");
    if (!fs::is_directory(source))
        throw err("config.yaml: \"source\" is not dir.");
    source = fs::canonical(source);

    if (!fs::exists(target) || !fs::is_directory(target)) {
        fs::create_directories(target);
        std::cout << "Config.target created: " << target << '\n';
    }
    target = fs::canonical(target);
}

void Config::print()
{
    std::cout
        << "Config.source    = " << source << "\n"
        << "Config.target    = " << target << "\n"
        << "Config.frequency = " << frequency << " (min) \n";
}


Daemon::Daemon(
    fs::path configPath)
{
    config = Config(configPath);
    std::cout << "Daemon member created \n";
    config.print();
}

void Daemon::backup()
{
    time_t now = sys_clock::to_time_t(sys_clock::now());
    std::stringstream date;
    date << std::put_time(std::localtime(&now), "%d_%m_%Y__%H_%M_%S");

    fs::path backupDir = config.target / date.str();
    fs::create_directories(backupDir);

    for (const auto& entry : fs::directory_iterator(config.source))
    {
        fs::copy(
            entry.path(),
            backupDir / entry.path().filename(),
            fs::copy_options::recursive | fs::copy_options::overwrite_existing
        );
    }

    std::cout << "Daemon.backup() done \n";
}
