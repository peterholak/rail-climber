#include "StaticPicture.h"

StaticPicture::StaticPicture(
    const std::string &texturePath,
    const Vec2 &position,
    const Vec2 &size,
    Alignment alignment
) : Widget(position, size, alignment), mOwnsTexture(true)
{
    mTexture.load(texturePath.c_str());
}


StaticPicture::StaticPicture(
    const Texture *texture,
    const Vec2 &position,
    const Vec2 &size,
    Widget::Alignment alignment
) : Widget(position, size, alignment), mExternalTexture(texture), mOwnsTexture(false)
{

}


void StaticPicture::draw(const BitmapRenderer &bitmapRenderer) {
    if (mOwnsTexture) {
        bitmapRenderer.renderTexture(mTexture, bottomLeft(), size());
    }else{
        bitmapRenderer.renderTexture(*mExternalTexture, bottomLeft(), size());
    }
}

void StaticPicture::reload() {

}

void StaticPicture::unload() {

}

void StaticPicture::changePicture(const std::string &texturePath) {
    if (mOwnsTexture) {
        mTexture.unload();
        mExternalTexture = nullptr;
    }
    mOwnsTexture = true;
    mTexture.load(texturePath.c_str());
}

void StaticPicture::changePicture(const Texture *texture) {
    if (mOwnsTexture) {
        mTexture.unload();
    }
    mOwnsTexture = false;
    mExternalTexture = texture;
}
