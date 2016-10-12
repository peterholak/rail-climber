#import "ios_core.h"
#include <string>

namespace RailClimberPlatform {
    
    void initialize() { }
    
    void log(int level, const char *fmt, va_list argList) { }
    
    /**
     * Load a file. The returned buffer has to be manually freed using delete[]
     */
    char *loadFile(const char *filename, int *lenret = 0) { return nullptr; }
    
    /**
     * Immediately exit the game in a platform-specific way
     */
    void quit() { }
    
    RCLSettingsType loadSettings(const char *filename) { return nullptr; }
    void unloadSettings(RCLSettingsType settings) { }
    void setSetting(RCLSettingsType settings, const char *section, const char *key, const char *value) { }
    std::string getSetting(RCLSettingsType settings, const char *section, const char *key) { return ""; }
    
    std::string getLanguageCode() { return ""; }
    void vibrate(int duration) { }
    long long getTicks() { return 0; }
    
}
