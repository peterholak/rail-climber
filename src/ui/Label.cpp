#include "Label.h"
#include "Logging.h"
#include "graphics/Font.h"
#include "graphics/BitmapRenderer.h"

using namespace std;

Label::Label(
    const std::string &text,
    const Vec2 &position,
    const Font *font,
    Color color,
    Widget::Alignment alignment,
    const Vec2 &size,
    Label::RenderingStrategy rendering
) : mText(text), mFont(font), mColor(color), Widget(position, size, alignment)
{
    mFont->createTextLabel(mTextTexture, mText, mColor);
    updateCachedSize();
}

Label::~Label() {
    unload();
}

void Label::setText(const string &newText) {
    if (mText == newText) {
        Log::info("Text is already set, skipping...");
        return;
    }
    mText = newText;
    reload();
}

void Label::draw(const BitmapRenderer &bitmapRenderer) {
    bitmapRenderer.renderTexture(mTextTexture, bottomLeft(), size());
}

void Label::reload() {
    mTextTexture.unload();
    mFont->createTextLabel(mTextTexture, mText.c_str(), mColor);
    updateCachedSize();
}

void Label::unload() {
    mTextTexture.unload();
}

void Label::setColor(const Color &newColor) {
    mColor = newColor;
    mTextTexture.unload();
    mFont->createTextLabel(mTextTexture, mText.c_str(), mColor);
}

Vec2 Label::calculateSize() const {
    return mCachedActualSize;
}

void Label::updateCachedSize() {
    float w, h;
    mFont->textMetrics(mText, &w, &h);
    mCachedActualSize.set(w, h);
}
