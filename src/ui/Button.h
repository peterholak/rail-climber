#pragma once

#include "ui/Widget.h"
#include <string>
#include "graphics/BitmapRenderer.h"
#include "graphics/Texture.h"
#include "3rdparty/nano-signal-slot/nano_signal_slot.hpp"
#include <cassert>
#include "graphics/Color.h"
#include "graphics/Font.h"

class Button : public Widget {
public:
    struct TextColorScheme {
        TextColorScheme() {}
        TextColorScheme(Color _baseText) : baseText(_baseText) { }
        Color baseText = Color::black;
        Color baseBackground = Color::transparent;
        Color downText = Color::black;
        Color downBackground = Color::white;
        Color disabledText = Color::white;
        Color disabledBackground = Color::transparent;
    };

    /** TextureSet that only refers to textures managed by some other object */
    struct TextureSet {
        TextureSet(const Texture *_base, const Texture *_down = nullptr, const Texture *_disabled = nullptr)
            : base(_base), down(_down), disabled(_disabled)
        {
            if (_down == nullptr) {
                down = _base;
            }

            if (_disabled == nullptr) {
                disabled = _base;
            }
        }
        const Texture *base;
        const Texture *down;
        const Texture *disabled;
    };

    /** Texture set that exclusively owns and manages the lifetime of the textures. Should be used for one-of-a-kind picture buttons. */
    struct OwnedTextureSet : public TextureSet {
        OwnedTextureSet(
            const std::string &basePath,
            const std::string &downPath = std::string(),
            const std::string &disabledPath = std::string()
        ) : TextureSet(new Texture(basePath))
        {
            if (!downPath.empty()) {
                down = new Texture(downPath);
            }

            if (!disabledPath.empty()) {
                disabled = new Texture(disabledPath);
            }
        }

        ~OwnedTextureSet() {
            delete base;
            if (down != base) {
                delete down;
            }
            if (disabled != base) {
                delete disabled;
            }
        }
    };

    enum ButtonType {
        Text = 1, Image = 2, TextAndImage = 1 | 2
    };

    enum UserDataType {
        IntData = 0, PointerData = 1
    };

    Button(
        const std::string &text,
        const Font *font,
        const Vec2 &position,
        const Vec2 &size = Vec2(Widget::SIZE_AUTO, Widget::SIZE_AUTO),
        TextColorScheme colorScheme = Color::black,
        Alignment alignment = BottomLeft,
        TextureSet *imageTextures = nullptr
    );

    Button(
        TextureSet *textures,
        const Vec2 &position,
        const Vec2 &size,
        Alignment alignment = BottomLeft
    );

    virtual void draw(const BitmapRenderer& bitmapRenderer);
    virtual void reload();
    virtual void unload();

    virtual bool wantsEventsOutsideOfBounds() const { return true; }
    virtual void handleTouchPress(const Vec2 &position);
    virtual void handleTouchMove(const Vec2 &position);
    virtual void handleTouchRelease(const Vec2 &position);

    void setEnabled(bool enabled) { mEnabled = enabled; }
    void setImageTextures(TextureSet *imageTextures) { mImageTextures = imageTextures; }
    void setText(const std::string& newText);

    void setUserData(int userData) { mUserDataType = IntData, mUserData.intValue = userData; }
    void setUserData(void *userData) { mUserDataType = PointerData; mUserData.pointerValue = userData; }
    int getIntUserData() { assert(mUserDataType == IntData); return mUserData.intValue; }
    void *getPointerUserData() { assert(mUserDataType == PointerData); return mUserData.pointerValue; }


    Nano::Signal<void(Button *sender)> clicked;

protected:
    virtual Vec2 calculateSize() const { return mCachedActualSize; }

private:
    std::string mText;
    ButtonType mButtonType;
    TextureSet *mImageTextures;

    Texture *mTextTextureBase = nullptr;
    Texture *mTextTextureDown = nullptr;
    Texture *mTextTextureDisabled = nullptr;

    TextColorScheme mTextColorScheme;
    const Font *mFont;
    bool mPressed = false;
    bool mEnabled = true;
    Vec2 mCachedActualSize;
    Vec2 mCachedTextSize;

    union {
        int intValue;
        void *pointerValue;
    } mUserData;
    UserDataType mUserDataType;

    const Texture &textureForCurrentState() const;
    const Texture &textTextureForCurrentState() const;
    const Color &backgroundForCurrentState() const;

    void updateCachedSize();
};
