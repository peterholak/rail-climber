#pragma once

#include <cinttypes>
#include <string>

#ifdef PLATFORM_SDL
#include "../platform_specific/sdl/sdl_graphics.h"
#endif

#ifdef PLATFORM_ANDROID
#include "../platform_specific/android/android_graphics.h"
#endif

#ifdef PLATFORM_TIZEN
#include "../platform_specific/tizen/tizen_graphics.h"
#endif

#ifdef PLATFORM_IOS
#include "../platform_specific/ios/ios_graphics.h"
#endif

#ifdef PLATFORM_EMSCRIPTEN
#include "../platform_specific/emscripten/emscripten_graphics.h"
#endif

namespace RailClimberPlatform {

    /**
     * Create a 32-bit RGBA texture from a png file
     * @param retW, retH where the size of the image in pixels will be stored, or NULL
     * @param hRepeat, vRepeat whether the texture repeats horizontally/vertically (sets clamp mode)
     * @return OpenGL texture id
     */
    unsigned int loadTexture(const std::string &filename, int *retW, int *retH, bool hRepeat, bool vRepeat);

    enum MultiLineJustify {
        Left, Center
    };

    RCLFontType loadFont(const std::string &filename, int ptsize);
    void unloadFont(RCLFontType font);
    unsigned int textureFromText(RCLFontType font, std::uint32_t colorRgba, const std::string &text, int *retW, int *retH);
    void textMetrics(RCLFontType font, const std::string &text, int *retW, int *retH);
    unsigned int multiLineText(RCLFontType font, std::uint32_t colorRgba, const std::string &text, int w, int *retH, MultiLineJustify justify = Left);

}
