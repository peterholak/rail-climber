#pragma once

#include "game_logic/Vec2.h"
#include "graphics/ShaderProgram.h"
#include <unordered_map>

class Rail;
class Theme;

class RailRenderer {
public:
    RailRenderer(const ShaderProgram &defaultProgram, const ShaderProgram &deadlyRailProgram);
    void render(const Rail *rail, const Theme *theme);

private:
    std::unordered_map<const Rail*, float**> mCache;
    const ShaderProgram &mDeadlyRailProgram;
    const ShaderProgram &mDefaultProgram;

public:
    static int textureId;
    static int disabledTextureId;
    static int deadlyRailProgramId;
};
