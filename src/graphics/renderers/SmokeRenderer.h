#pragma once

#include "graphics/ShaderProgram.h"
#include "PolygonalObjectRenderer.h"

class SmokeRenderer : public PolygonalObjectRenderer {
public:
    SmokeRenderer(const ShaderProgram &defaultProgram, const ShaderProgram &hazardProgram);
    void render(const StaticObject *object, Theme *theme);

private:
    const ShaderProgram &mHazardProgram;

public:
    static constexpr int smokeFrames = 64;
    static int textureId[smokeFrames];
    static int smokeProgramId;
};
