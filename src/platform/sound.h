#pragma once

#include <string>

namespace RailClimberPlatform {
    void soundInit();
    void *loadSound(const std::string &filename);
    void playSound(void *data);
    void unloadSound(void *data);
    void soundCleanup();
    int getVolume();
    void setVolume(int vol);
}
