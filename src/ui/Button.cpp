#include "Button.h"
#include "platform/graphics.h"
#include "MenuScreen.h"
#include "Logging.h"
#include "InvalidCoordinatesException.h"

using namespace std;

Button::Button(
    const string &text,
    const Font *font,
    const Vec2 &position,
    const Vec2 &size,
    TextColorScheme textColors,
    Alignment alignment,
    TextureSet *imageTextures
) : mText(text), mFont(font), mTextColorScheme(textColors), mImageTextures(imageTextures), Widget(position, size, alignment)
{
    setText(mText);
}

Button::Button(TextureSet *textures, const Vec2 &position, const Vec2 &size, Alignment alignment)
    : mImageTextures(textures), mButtonType(Image), Widget(position, size, alignment)
{
    updateCachedSize();
}

void Button::draw(const BitmapRenderer& bitmapRenderer) {
    if (mButtonType & Image) {
        bitmapRenderer.renderTexture(textureForCurrentState(), bottomLeft(), size());
    }

    if (mButtonType & Text) {
        Color background = backgroundForCurrentState();
        if (!(mButtonType & Image) && !background.isTransparent()) {
            bitmapRenderer.renderSolidRectangle(bottomLeft(), size(), background);
        }
        Vec2 textPosition = Vec2(
            bottomLeft().x() + width() / 2.0f - mCachedTextSize.x() / 2.0f,
            bottomLeft().y() + height() / 2.0f - mCachedTextSize.y() / 2.0f
        );
        bitmapRenderer.renderTexture(textTextureForCurrentState(), textPosition, mCachedTextSize);
    }
}

void Button::reload() {

}

void Button::unload() {

}

const Texture &Button::textureForCurrentState() const {
    return *(mEnabled ? (mPressed ? mImageTextures->down : mImageTextures->base) : mImageTextures->disabled);
}

const Texture &Button::textTextureForCurrentState() const {
    return *(mEnabled ? (mPressed ? mTextTextureDown : mTextTextureBase) : mTextTextureDisabled);
}

const Color &Button::backgroundForCurrentState() const {
    return mEnabled ? (mPressed ? mTextColorScheme.downBackground : mTextColorScheme.baseBackground) : mTextColorScheme.disabledBackground;
}

void Button::handleTouchPress(const Vec2 &position) {
    if (mEnabled && isPositionWithinBounds(position)) {
        mPressed = true;
    }
}

void Button::handleTouchMove(const Vec2 &position) {

}

void Button::handleTouchRelease(const Vec2 &position) {
    if (mEnabled && mPressed && isPositionWithinBounds(position)) {
        clicked(this);
    }
    mPressed = false;
}

void Button::updateCachedSize() {
    if (!(mButtonType & Text) && mRequestedCoordinates.size == Vec2(Widget::SIZE_AUTO, Widget::SIZE_AUTO)) {
        throw InvalidCoordinatesException("Width and height of a widget can not both be auto for image-only buttons");
    }

    if (mButtonType & Text) {
        float textWidth, textHeight;
        mFont->textMetrics(mText, &textWidth, &textHeight);
        mCachedTextSize.set(textWidth, textHeight);

        if (mRequestedCoordinates.size == Vec2(Widget::SIZE_AUTO, Widget::SIZE_AUTO)) {
            mCachedActualSize.set(textWidth, textHeight);
            return;
        }
    }

    const Texture *texture = (mButtonType & Image ? mImageTextures->base : mTextTextureBase);
    if (mRequestedCoordinates.size.x() == SIZE_AUTO) {
        mCachedActualSize.set(mRequestedCoordinates.size.y() * texture->aspectRatio, mRequestedCoordinates.size.y());
    }else if (mRequestedCoordinates.size.y() == SIZE_AUTO) {
        mCachedActualSize.set(mRequestedCoordinates.size.x(), mRequestedCoordinates.size.x() / texture->aspectRatio);
    }else{
        mCachedActualSize = mRequestedCoordinates.size;
    }
}

void Button::setText(const std::string &newText) {
    if (mImageTextures == nullptr) {
        mButtonType = Text;
    }else{
        mButtonType = TextAndImage;
    }

    mText = newText.empty() ? " " : newText;
    if (newText.empty()) {
        Log::error("Setting empty text to button");
    }

    delete mTextTextureBase;
    if (mTextTextureDown != mTextTextureBase) {
        delete mTextTextureDown;
    }
    if (mTextTextureDisabled != mTextTextureBase) {
        delete mTextTextureDisabled;
    }

    mTextTextureBase = new Texture();
    mFont->createTextLabel(*mTextTextureBase, mText, mTextColorScheme.baseText);

    if (mTextColorScheme.downText != mTextColorScheme.baseText) {
        mTextTextureDown = new Texture();
        mFont->createTextLabel(*mTextTextureDown, mText, mTextColorScheme.downText);
    }else{
        // no need to have a separate texture if the color (and therefore the contents) is exactly the same
        mTextTextureDown = mTextTextureBase;
    }

    if (mTextColorScheme.disabledText != mTextColorScheme.baseText) {
        mTextTextureDisabled = new Texture();
        mFont->createTextLabel(*mTextTextureDisabled, mText, mTextColorScheme.disabledText);
    }else{
        mTextTextureDisabled = mTextTextureBase;
    }

    updateCachedSize();
}
