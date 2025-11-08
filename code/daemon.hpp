
#include <filesystem>
namespace fs = std::filesystem;
#include <yaml-cpp/yaml.h>
#include <iostream>


class Config
{
    fs::path source;
    fs::path target;
    uint16_t frequency;

    bool getFrequency(const std::string freqStr);

public:

    Config() = default;
    Config(fs::path configPath);
    void print();
};


class Daemon
{
    Config config;

public:

    Daemon(fs::path configPath);
    void backup();  // todo
    void run();     // todo
};
