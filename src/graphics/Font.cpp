#include "Font.h"
#include "game_logic/UnitConverter.h"
#include "Logging.h"
#include "platform/graphics.h"
#include <cmath>

using namespace std;

Font::Font(const string &filename, float inGameHeight, const UnitConverter &unitConverter)
    : mFont(nullptr), mUnitConverter(unitConverter), mInGameHeight(inGameHeight)
{
    load(filename, int(ceil(unitConverter.pixelLength(inGameHeight))));
}

Font::~Font() {
    unload();
}

void Font::load(const string &filename, int ptsize) {
    if (mFont) {
        Log::warn("Font '%s' already loaded, unloading...", filename.c_str());
        unload();
    }
    mFont = RailClimberPlatform::loadFont(filename, ptsize);
}

void Font::unload() {
    if (mFont) {
        RailClimberPlatform::unloadFont(mFont);
    }
    mFont = nullptr;
}

void Font::createTextLabel(Texture &target, const string &text, Color color, bool multiLine, float multilineWidth, TextJustify multilineJustify) const {
    GLuint texId = 0;
    int retW, retH;
    if (!multiLine) {
        texId = RailClimberPlatform::textureFromText(mFont, color.rgba(), text, &retW, &retH);
    } else {
        if (multilineWidth < 0.00001) {
            Log::warn("Zero multiline width, text: '%s'", text.c_str());
            multilineWidth = 1.0;
        }

        int pixelWidth = mUnitConverter.pixelLength(multilineWidth);
        retW = pixelWidth;

        texId = RailClimberPlatform::multiLineText(mFont, color.rgba(), text, pixelWidth, &retH,
                multilineJustify == Left ? RailClimberPlatform::Left : RailClimberPlatform::Center);
    }
    target.fromGLTexture(texId, retW, retH);
}

float Font::textWidth(const string &text) const {
    float w;
    textMetrics(text, &w, nullptr);
    return w;
}

float Font::textHeight(const string &text) const {
    // TODO: how does this compare with default height
    float h;
    textMetrics(text, nullptr, &h);
    return h;
}

void Font::textMetrics(const string &text, float *w, float *h) const {
    int tmpW, tmpH;
    RailClimberPlatform::textMetrics(mFont, text, &tmpW, &tmpH);
    if (w != nullptr) {
        *w = mUnitConverter.inGameLength(tmpW);
    }
    if (h != nullptr) {
        *h = mUnitConverter.inGameLength(tmpH);
    }
}
