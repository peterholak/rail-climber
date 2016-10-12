#pragma once

#include "ui/Widget.h"
#include "graphics/Texture.h"
class BitmapRenderer;

class StaticPicture : public Widget {

public:
    StaticPicture(
        const std::string &texturePath,
        const Vec2 &position,
        const Vec2 &size,
        Alignment alignment = BottomLeft
    );

    StaticPicture(
        const Texture *texture,
        const Vec2 &position,
        const Vec2 &size,
        Alignment alignment = BottomLeft
    );

    void changePicture(const std::string &texturePath);
    void changePicture(const Texture *texture);

    virtual void draw(const BitmapRenderer &bitmapRenderer);
    virtual void reload();
    virtual void unload();

private:
    Texture mTexture;
    const Texture *mExternalTexture = nullptr;
    bool mOwnsTexture;
};
