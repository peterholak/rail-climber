#include "Sound.h"
#include "Logging.h"
#include "platform/sound.h"

using namespace std;

Sound *Sound::mInstance = 0;

Sound::Sound() {
    enabled = false;
    isSilent = false;
}

Sound *Sound::getInstance() {
    if (!mInstance) {
        mInstance = new Sound();
    }
    return mInstance;
}

void Sound::initialize() {
    RailClimberPlatform::soundInit();
    enabled = false;
    isSilent = false;
}

void Sound::cleanup() {
    for (map<string, void*>::iterator it = mSounds.begin(); it != mSounds.end(); ++it) {
        RailClimberPlatform::unloadSound(it->second);
    }
    mSounds.clear();
    RailClimberPlatform::soundCleanup();
    delete mInstance;
    mInstance = 0;
}

bool Sound::loadSound(const string &soundname, const string &filename) {
    Log::info("Loading sound %s from %s", soundname.c_str(), filename.c_str());
    void *sound = RailClimberPlatform::loadSound(filename);
    if (!sound) {
        return false;
    }
    mSounds[soundname] = sound;
    return true;
}

void Sound::playSound(const string &soundname) {
    if (enabled && !isSilent && mSounds.find(soundname) != mSounds.end()) {
        RailClimberPlatform::playSound(mSounds[soundname]);
    }
}

void Sound::unloadSound(const string &soundname) {
    if (mSounds.find(soundname) != mSounds.end()) {
        RailClimberPlatform::unloadSound(mSounds[soundname]);
    }
}
