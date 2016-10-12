#include "RailRenderer.h"
#include "graphics/Theme.h"
#include "game_logic/Rail.h"
#include <vector>
#include <cmath>

using std::unordered_map;
using std::vector;

int RailRenderer::deadlyRailProgramId;

RailRenderer::RailRenderer(const ShaderProgram &defaultProgram, const ShaderProgram &deadlyRailProgram) :
    mDefaultProgram(defaultProgram), mDeadlyRailProgram(deadlyRailProgram)
{

}

void RailRenderer::render(const Rail *r, const Theme *theme) {
    GLfloat *texcoord = nullptr;
    GLfloat *color = nullptr;
    GLfloat *rail = nullptr;

    const vector<Vec2> &points = r->getPoints();
    int nSegments = points.size() - 1;
    GLfloat thick = 0.055f;

    if (mCache.find(r) == mCache.end()) {
        mCache[r] = new float*[3];
        mCache[r][0] = nullptr;
        mCache[r][1] = nullptr;
        mCache[r][2] = nullptr;
    }

    if (mCache[r][2] == nullptr) {
        mCache[r][2] = new float[nSegments * 12];
        texcoord = mCache[r][2];
        for (int i = 0; i < nSegments; i++) {
            texcoord[i * 12] = texcoord[i * 12 + 1] = texcoord[i * 12 + 3] = texcoord[i * 12 + 4] = texcoord[i * 12 + 7] = texcoord[i * 12 + 8] = 0.0f;
            texcoord[i * 12 + 2] = texcoord[i * 12 + 6] = texcoord[i * 12 + 10] = 2.0f;
            texcoord[i * 12 + 5] = texcoord[i * 12 + 9] = texcoord[i * 12 + 11] = 1.0f;
        }
    } else {
        texcoord = mCache[r][2];
    }

    //cache the "box around line" coordinates for the rail
    //TODO: make a vbo buffer to get even more speed
    if (mCache[r][0] == nullptr) {
        mCache[r][0] = new float[nSegments * 18];
        rail = mCache[r][0];
        for (int i = 0; i < nSegments; i++) {
            Vec2 r1, r2, r3, r4;
            float x1 = points[i].x(), y1 = points[i].y();
            float x2 = points[i + 1].x(), y2 = points[i + 1].y();
            float dist = std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))*3; //used to make textures the same size on all rails
            Vec2::boxAroundLine(thick, Vec2(x1, y1), Vec2(x2, y2), r1, r2, r3, r4);
            texcoord[i * 12 + 2] = texcoord[i * 12 + 6] = texcoord[i * 12 + 10] = dist;
            rail[i * 18 + 2] = rail[i * 18 + 5] = rail[i * 18 + 8] = rail[i * 18 + 11] = rail[i * 18 + 14] = rail[i * 18 + 17] = 0.0f;
            rail[i * 18 + 0] = r1.x();
            rail[i * 18 + 1] = r1.y();
            rail[i * 18 + 3] = r2.x();
            rail[i * 18 + 4] = r2.y();
            rail[i * 18 + 6] = r3.x();
            rail[i * 18 + 7] = r3.y();
            rail[i * 18 + 9] = rail[i * 18 + 3];
            rail[i * 18 + 10] = rail[i * 18 + 4];
            rail[i * 18 + 12] = rail[i * 18 + 6];
            rail[i * 18 + 13] = rail[i * 18 + 7];
            rail[i * 18 + 15] = r4.x();
            rail[i * 18 + 16] = r4.y();
        }
    } else {
        rail = mCache[r][0];
    }

    if (mCache[r][1] == nullptr) {
        mCache[r][1] = new float[nSegments * 24];
        color = mCache[r][1];
        for (int i = 0; i < nSegments * 24; i++) {
            color[i] = 1;
        }
    } else {
        color = mCache[r][1];
    }

    if (r->deadly) {
        mDeadlyRailProgram.bind();
        mDeadlyRailProgram.setPosition(rail);
        mDeadlyRailProgram.setTextureCoordinates(texcoord);
        // TODO: this causes issues?? check return values on everything gl in debug mode...
        //mDeadlyRailProgram.setColor(color);
        mDeadlyRailProgram.setUniform("drProgress", (r->deadlyStage - 1) * r->deadlyProgress, 0, (r->deadlyStage - 2) * -0.8f, (r->disabled ? -0.4f : 0));
    } else {
        mDefaultProgram.bind();
        mDefaultProgram.setPosition(rail);
        mDefaultProgram.setColor(color);
        mDefaultProgram.setTextureCoordinates(texcoord);
    }

    if (r->disabled) {
        theme->getTexture("rail-disabled").bind();
    } else {
        theme->getTexture("rail").bind();
    }
    glDrawArrays(GL_TRIANGLES, 0, nSegments * 6);
    mDefaultProgram.bind();
}
