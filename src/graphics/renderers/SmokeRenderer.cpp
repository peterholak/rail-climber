#include "SmokeRenderer.h"
#include "game_logic/StaticObject.h"
#include "graphics/Theme.h"
#include "graphics/ShaderProgram.h"

int SmokeRenderer::smokeProgramId;

void SmokeRenderer::render(const StaticObject *object, Theme *theme) {
    int nPoints = object->drawPoints.size();
    object->fillPointCache(theme->getTexture("noise").scaling);
    object->calculatePolygonBuffer(theme->getTexture("noise").scaling); //doesn't do anything if cache is already filled

    mHazardProgram.bind();
    mHazardProgram.setPosition(object->pointPos);
    mHazardProgram.setTextureCoordinates(object->pointTex);
    mHazardProgram.setColor(object->pointColor);

    theme->getTexture("noise").bind();

    glDrawArrays(GL_TRIANGLES, 0, nPoints);

    mHazardProgram.setPosition(object->bufferPos);
    mHazardProgram.setColor(object->bufferColor);
    mHazardProgram.setTextureCoordinates(object->bufferTex);

    glDrawArrays(GL_TRIANGLES, 0, object->bufferPointCount);

    mDefaultProgram.bind();
}

SmokeRenderer::SmokeRenderer(const ShaderProgram &defaultProgram, const ShaderProgram &hazardProgram) : PolygonalObjectRenderer(defaultProgram), mHazardProgram(hazardProgram) {

}
