#include "WorldRenderer.h"
#include "game_logic/World.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"
#include "graphics/BitmapRenderer.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Theme.h"

#define RECTANGLE3(x,y,w,h) {x,y,0,x+(w),y,0,x,y+(h),0,x+(w),y+(h),0}
#define RGBA4(r,g,b,a) {r,g,b,a,r,g,b,a,r,g,b,a,r,g,b,a}
#define SETFIELDIN4x4(a,i,v) {a[(i)] = a[(i)+4] = a[(i)+8] = a[(i)+12] = v;}
#define RECTANGLE_TEXCOORD(w,h) {0.0f,0.0f,w,0.0f,0.0f,h,w,h}

WorldRenderer::WorldRenderer(const UnitConverter &unitConverter, const SharedAssetRegistry &assetRegistry)
    : mUnitConverter(unitConverter), mAssetRegistry(assetRegistry)
{

}

void WorldRenderer::drawPlayer(const World *world, Theme *theme) {
    const Vec2 &player = world->getPlayer().pos;
    float x = player.x(), y = player.y();

    theme->getTexture("ball").bind();
    float r = World::playerRadius;
    if (world->getPlayer().state == World::DeadState) {
        r = World::playerRadius - 0.01 * world->getPlayer().deadCounter;
    }
    GLfloat color[] = RGBA4(1, 1, 1, 1);
    GLfloat texcoord[] = RECTANGLE_TEXCOORD(1.0f, 1.0f);
    float rot = world->getPlayer().body->GetAngle();
    float sinrot = sin(rot);
    float cosrot = cos(rot);
    GLfloat pts[] = RECTANGLE3(x - r, y - r, 2 * r, 2 * r);
    for (int i = 0; i < 4; i++) {
        float xx = pts[i * 3] - x, yy = pts[i * 3 + 1] - y;
        float xt = xx * cosrot - yy * sinrot;
        float yt = xx * sinrot + yy * cosrot;
        pts[i * 3] = xt + x;
        pts[i * 3 + 1] = yt + y;
    }
    GLfloat pts2[] = RECTANGLE3(x - r, y + r, 2 * r, -2 * r);
    const ShaderProgram& defaultProgram = mAssetRegistry.defaultProgram();
    defaultProgram.setPosition(pts);
    defaultProgram.setTextureCoordinates(texcoord);
    defaultProgram.setColor(color);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    defaultProgram.setPosition(pts2);
    theme->getTexture("glare").bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void WorldRenderer::drawFinishLine(const World *world, Theme *theme) {
    float pixelHeight = mUnitConverter.pixelLength(mUnitConverter.gameHeight());
    // TODO: is this even necessary? texture has to line up to be squareish but maybe it could be done simpler? using texture scaling?
    const Texture &texFinish = theme->getTexture("finish");
    GLfloat finHRatio = texFinish.height * 12.5 / pixelHeight;
    GLfloat texWFinish = (pixelHeight * (world->levelWidth / mUnitConverter.gameWidth())) / texFinish.width * finHRatio;

    GLfloat fLevelH = world->levelH, fLevelW = world->levelWidth;
    GLfloat finish[] = RECTANGLE3(0.0f, fLevelH, fLevelW, 0.8f);
    GLfloat finishtex[] = RECTANGLE_TEXCOORD(texWFinish, 1);

    //TODO: only if scroll is such that finish is visible (same for floor etc.)
    texFinish.bind();
    mAssetRegistry.defaultProgram().setPosition(finish);
    mAssetRegistry.defaultProgram().setTextureCoordinates(finishtex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void WorldRenderer::drawFloor(const World *world, Theme *theme) {
    float pixelWidth = mUnitConverter.pixelLength(mUnitConverter.gameWidth());
    const Texture &texFloor = theme->getTexture("wall");
    GLfloat texWFloor = (pixelWidth * (world->levelWidth / mUnitConverter.gameWidth())) / texFloor.width;
    GLfloat fLevelH = world->levelH, fLevelW = world->levelWidth;

    float floorHeight = World::wallThickness;
    GLfloat texcoordFloor[] = RECTANGLE_TEXCOORD(texWFloor, 1.0f);
    GLfloat floor[] = RECTANGLE3(0.0f, 0.0f, fLevelW, floorHeight);
    texFloor.bind();
    mAssetRegistry.defaultProgram().setPosition(floor);
    mAssetRegistry.defaultProgram().setTextureCoordinates(texcoordFloor);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
