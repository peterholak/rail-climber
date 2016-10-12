#pragma once

#include <memory>
#include <string>
#include "platform/graphics.h"
#include "graphics/Texture.h"
#include "graphics/Color.h"

class Painter;
class UnitConverter;

class Font {
public:
    Font(const std::string &filename, float inGameHeight, const UnitConverter &unitConverter);

    Font(const Font& other) = delete;
    Font& operator=(const Font& other) = delete;

    ~Font();

    enum TextJustify {
        Left, Center
    };

    void load(const std::string &filename, int ptsize);
    void unload();

    void createTextLabel(Texture &target, const std::string &text, Color color = Color::black, bool multiLine = false,
                         float multilineWidth = 0, TextJustify multilineJustify = Left) const;
    float textWidth(const std::string &text) const;
    float textHeight(const std::string &text) const;
    void textMetrics(const std::string &text, float *w, float *h) const;

    float getInGameHeight() const { return mInGameHeight; }

private:
    Painter *mPainter;
    RailClimberPlatform::RCLFontType mFont;
    const UnitConverter &mUnitConverter;
    float mInGameHeight;
};
