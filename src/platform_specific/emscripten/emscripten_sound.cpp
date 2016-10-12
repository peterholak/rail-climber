#include <string>

namespace RailClimberPlatform {
    void soundInit() { }
    void *loadSound(const std::string &filename) { return nullptr; }
    void playSound(void *data) { }
    void unloadSound(void *data) { }
    void soundCleanup() { }
    int getVolume() { return 0; }
    void setVolume(int vol) { }
}
