#pragma once

#include "graphics/renderers/PolygonalObjectRenderer.h"

class StaticObject;
class Theme;
class ShaderProgram;

class WallRenderer : public PolygonalObjectRenderer {
public:
    WallRenderer(const ShaderProgram &defaultProgram) : PolygonalObjectRenderer(defaultProgram) { }

private:
public:
    static int textureId;
};
