#pragma once

#include "graphics/ShaderProgram.h"
#include "game_logic/Vec2.h"

class WaveGenerator;
class Theme;
class ShaderProgram;

class WaveGeneratorRenderer {
public:
    WaveGeneratorRenderer(const ShaderProgram &defaultProgram);
    void render(const WaveGenerator *generator, const Theme *theme, const Vec2 &scroll);

private:
    const ShaderProgram &mDefaultProgram;
    float *mCos, *mSin;

public:
    static int activeTextureId;
    static int inactiveTextureId;
};
