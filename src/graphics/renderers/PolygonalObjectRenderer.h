#pragma once

class ShaderProgram;
class StaticObject;
class Theme;

class PolygonalObjectRenderer {
public:
    PolygonalObjectRenderer(const ShaderProgram &defaultProgram);
    void render(const StaticObject *object, Theme *theme);

protected:
    const ShaderProgram &mDefaultProgram;
};
