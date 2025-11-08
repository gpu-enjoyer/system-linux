
#include <filesystem>
namespace fs = std::filesystem;

#include <yaml-cpp/yaml.h>

#include <chrono>
using sys_clock = std::chrono::system_clock;

#include <iostream>


class Config
{
    friend class Daemon;
    
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

    Daemon() = default;
    Daemon(fs::path configPath);
    void backup();
};
