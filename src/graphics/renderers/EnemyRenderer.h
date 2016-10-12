#pragma once

#include "graphics/BitmapRenderer.h"

class Bullet;
class Enemy;
class Theme;
class ShaderProgram;
class SharedAssetRegistry;

class EnemyRenderer {
public:
    EnemyRenderer(const SharedAssetRegistry &assetRegistry, const BitmapRenderer &bitmapRenderer);
    void render(const Enemy *enemy, const Theme *theme);
    void renderBullet(const Bullet *bullet, const Theme *theme);

private:
    const SharedAssetRegistry &mAssetRegistry;
    const BitmapRenderer &mBitmapRenderer;

public:
    // TODO: verify if this is correct
    enum EyePosition {
        EyesTopLeft = 0,
        EyesTopRight = 1,
        EyesBottomRight = 2,
        EyesBottomLeft = 3
    };

    static int mouthOpenTextureId[4];
    static int mouthClosedTextureId[4];
};
