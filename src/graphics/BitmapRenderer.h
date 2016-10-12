#pragma once

#include "game_logic/Vec2.h"

class Texture;
class ShaderProgram;
class Color;

class BitmapRenderer {
public:
    BitmapRenderer(const ShaderProgram &shaderProgram);

    void renderTexture(const Texture &texture, float left, float bottom, float width, float height = SIZE_AUTO) const;
    void renderTexture(const Texture &texture, const Vec2 &bottomLeft, const Vec2 &size) const {
        renderTexture(texture, bottomLeft.x(), bottomLeft.y(), size.x(), size.y());
    }
    void renderSolidRectangle(const Vec2 &bottomLeft, const Vec2 &size, const Color &color) const;

    static constexpr float SIZE_AUTO = 0.0f;

private:
    const ShaderProgram &mShaderProgram;
};
