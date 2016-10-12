#pragma once

#include <map>
#include <string>
struct SDL_Window;

namespace RailClimberPlatform {

    extern std::string dataDir;

    struct RCLSDLSettings {
        std::string filename;
        std::map<std::string, std::string> settings;
    };
    typedef RCLSDLSettings* RCLSettingsType;

    extern bool g_quit;
    extern SDL_Window *g_sdlWindow;
}
