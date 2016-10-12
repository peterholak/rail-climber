#import "ios_graphics.h"

namespace RailClimberPlatform {
    
    /**
     * Create a 32-bit RGBA texture from a png file
     * @param retW, retH where the size of the image in pixels will be stored, or NULL
     * @param hRepeat, vRepeat whether the texture repeats horizontally/vertically (sets clamp mode)
     * @return OpenGL texture id
     */
    unsigned int loadTexture(const char *filename, int *retW, int *retH, bool hRepeat, bool vRepeat) {
        return 0;
    }
    
    enum MultiLineJustify {
        Left, Center
    };
    
    RCLFontType loadFont(const char *filename, int ptsize) {
        return nullptr;
    }
    
    void unloadFont(RCLFontType font) { }
    unsigned int textureFromText(RCLFontType font, int color, const char *text, int *retW, int *retH) { return 0; }
    void textMetrics(RCLFontType font, const char *text, int *retW, int *retH) { }
    unsigned int multiLineText(RCLFontType font, int color, const char *text, int w, int *retH, MultiLineJustify justify) { return 0; }
    
}
