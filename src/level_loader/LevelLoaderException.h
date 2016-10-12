#pragma once

#include <stdexcept>

class LevelLoaderException : public std::runtime_error {
public:
    LevelLoaderException(const char *what_arg) : std::runtime_error(what_arg) { }
    LevelLoaderException(const std::string &what_arg) : std::runtime_error(what_arg) { }
};
