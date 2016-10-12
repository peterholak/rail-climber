#pragma once

#include <SDL_opengles2.h>

// TODO: platform-specific opengl-only include??

struct _TTF_Font;

namespace RailClimberPlatform {
    typedef struct _TTF_Font* RCLFontType;
}