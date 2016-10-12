#pragma once

#include <string>
#include <stdarg.h>

#ifdef PLATFORM_SDL
#include "../platform_specific/sdl/sdl_core.h"
#endif

#ifdef PLATFORM_ANDROID
#include "../platform_specific/android/android_core.h"
#endif

#ifdef PLATFORM_TIZEN
#include "../platform_specific/tizen/tizen_core.h"
#endif

#ifdef PLATFORM_IOS
#include "../platform_specific/ios/ios_core.h"
#endif

#ifdef PLATFORM_EMSCRIPTEN
#include "../platform_specific/emscripten/emscripten_core.h"
#endif

namespace RailClimberPlatform {

    void initialize();

    void log(int level, const char *fmt, va_list argList);

    /**
     * Load a file. The returned buffer has to be manually freed using delete[]
     */
    char *loadFile(const char *filename, int *lenret = 0);

    /**
     * Immediately exit the game in a platform-specific way
     */
    void quit();

    RCLSettingsType loadSettings(const char *filename);
    void unloadSettings(RCLSettingsType settings);
    void setSetting(RCLSettingsType settings, const char *section, const char *key, const char *value);
    std::string getSetting(RCLSettingsType settings, const char *section, const char *key);

    std::string getLanguageCode();
    void vibrate(int duration);
    long long getTicks();

}
