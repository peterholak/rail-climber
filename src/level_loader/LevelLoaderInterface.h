#pragma once

#include <string>

class World;

class LevelLoaderInterface {
public:
    virtual World* loadLevel(const std::string &data, int version = 1) = 0;
};
