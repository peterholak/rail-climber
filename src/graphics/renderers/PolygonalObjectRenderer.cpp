#include "PolygonalObjectRenderer.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Theme.h"
#include "game_logic/StaticObject.h"

PolygonalObjectRenderer::PolygonalObjectRenderer(const ShaderProgram &defaultProgram) : mDefaultProgram(defaultProgram) {

}

void PolygonalObjectRenderer::render(const StaticObject *object, Theme *theme) {
    int nPoints = object->drawPoints.size();
    mDefaultProgram.bind();
    object->fillPointCache(theme->getTexture("wall").scaling);

    mDefaultProgram.setPosition(object->pointPos);
    mDefaultProgram.setTextureCoordinates(object->pointTex);
    mDefaultProgram.setColor(object->pointColor);

    theme->getTexture("wall").bind();
    glDrawArrays(GL_TRIANGLES, 0, nPoints);
}
