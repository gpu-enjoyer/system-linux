
#include <yaml-cpp/yaml.h>
#include <filesystem>
namespace fs = std::filesystem;
#include <iostream>

class Config
{
    fs::path source;
    fs::path target;
    uint16_t frequency;

    bool getFrequency(const std::string freqStr);

public:

    Config(const fs::path configPath);
    void print();
};
