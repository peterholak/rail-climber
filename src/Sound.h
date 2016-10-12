#pragma once

#include <map>
#include <string>

class Sound {
private:
    Sound();
    static Sound *mInstance;

    std::map<std::string, void*> mSounds;

public:
    static Sound *getInstance();

    void initialize();
    bool loadSound(const std::string &soundname, const std::string &filename);
    void playSound(const std::string &soundname);
    void unloadSound(const std::string &soundname);
    void cleanup();

    bool enabled; //in-game setting, whether sound is enabled
    bool isSilent; //whether the phone is in silent mode (regardless of in-game settings)
};
