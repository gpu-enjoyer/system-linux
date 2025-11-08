
#include "config.hpp"
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
    configPath = fs::absolute(fs::path(__FILE__)).parent_path() / configPath;
    if (!fs::exists(configPath)) throw err("config.yaml does not exist");

    std::string freqStr;

    try
    {
        YAML::Node configYaml = YAML::LoadFile(configPath);
        source  = configYaml["source"].as<std::string>();
        target  = configYaml["target"].as<std::string>();
        freqStr = configYaml["frequency"].as<std::string>();
    }
    catch (const YAML::BadFile& e) { throw err("config.yaml: bad file: " + configPath.string()); }
    catch (const YAML::ParserException& e) { throw err("config.yaml: parser exception: " + configPath.string()); }

    if (!getFrequency(freqStr)) throw err("config.yaml: invalid \"frequency\" value");

    fs::path baseDir = fs::absolute(configPath).parent_path();
    source = baseDir / source;
    target = baseDir / target;

    if (!fs::exists(source)) throw err("config.yaml: \"source\" does not exist");
    if (!fs::is_directory(source)) throw err("config.yaml: \"source\" is not dir.");
    source = fs::canonical(source);

    if (!fs::exists(target)) throw err("config.yaml: \"target\" does not exist");
    if (!fs::is_directory(target)) throw err("config.yaml: \"target\" is not dir.");
    target = fs::canonical(target);
}

void Config::print()
{
    std::cout
        << "\n source    = " << source
        << "\n target    = " << target
        << "\n frequency = " << frequency << " (min)"
        << std::endl;
}
