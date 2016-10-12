#pragma once

struct SDL_Window;

namespace RailClimberPlatform {
    typedef void* RCLSettingsType;

    extern bool g_quit;
    extern SDL_Window *g_sdlWindow;
}