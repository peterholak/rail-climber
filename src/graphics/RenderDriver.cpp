#include "RenderDriver.h"
#include "graphics/renderers/RailRenderer.h"
#include "graphics/Theme.h"
#include "graphics/renderers/WorldRenderer.h"
#include "graphics/renderers/WallRenderer.h"
#include "graphics/renderers/WaveGeneratorRenderer.h"
#include "graphics/renderers/SmokeRenderer.h"
#include "graphics/renderers/EnemyRenderer.h"
#include "SharedAssetRegistry.h"
#include "game_logic/GameScene.h"

RenderDriver::RenderDriver(const UnitConverter &unitConverter, SharedAssetRegistry *assetRegistry, const BitmapRenderer &bitmapRenderer)
    : mUnitConverter(unitConverter)
{
    mRailRenderer = new RailRenderer(assetRegistry->defaultProgram(), assetRegistry->getProgram(RailRenderer::deadlyRailProgramId));
    mWaveGeneratorRenderer = new WaveGeneratorRenderer(assetRegistry->defaultProgram());
    mWorldRenderer = new WorldRenderer(mUnitConverter, *assetRegistry);
    mWallRenderer = new WallRenderer(assetRegistry->defaultProgram());
    mSmokeRenderer = new SmokeRenderer(assetRegistry->defaultProgram(), assetRegistry->getProgram(SmokeRenderer::smokeProgramId));
    mEnemyRenderer = new EnemyRenderer(*assetRegistry, bitmapRenderer);
    mAssetRegistry = assetRegistry;

    mTheme = new Theme();
    mTheme->tempPutTexture("rail", Texture::Repeat);
    mTheme->tempPutTexture("rail-disabled", Texture::Repeat);
    mTheme->tempPutTexture("wave-generator-active");
    mTheme->tempPutTexture("wave-generator-inactive");
    mTheme->tempPutTexture("wall", Texture::Repeat);
    mTheme->tempPutTexture("noise", Texture::Repeat, "noise/noise001");
    mTheme->tempPutTexture("enemy", Texture::DoNotRepeat, "enemy_1_c");
    mTheme->tempPutTexture("enemy-fast", Texture::DoNotRepeat, "enemy_f_1_c");
    mTheme->tempPutTexture("bullet");
    mTheme->tempPutTexture("background", Texture::Repeat);
    mTheme->tempPutTexture("background-top", Texture::Repeat);
    mTheme->tempPutTexture("ball");
    mTheme->tempPutTexture("glare");
    mTheme->tempPutTexture("finish", Texture::Repeat);
}

void RenderDriver::drawRail(const Rail *rail) {
    mRailRenderer->render(rail, mTheme);
}

void RenderDriver::drawStaticObject(const StaticObject *staticObject) {
    if (staticObject->hazard) {
        mSmokeRenderer->render(staticObject, mTheme);
    }else {
        mWallRenderer->render(staticObject, mTheme);
    }
}

void RenderDriver::drawWaveGenerator(const WaveGenerator *waveGenerator) {
    mWaveGeneratorRenderer->render(waveGenerator, mTheme, Vec2());
}

void RenderDriver::drawGrass(const Grass *grass) {

}

void RenderDriver::drawEnemy(const Enemy *enemy) {
    mEnemyRenderer->render(enemy, mTheme);
}

void RenderDriver::drawBullet(const Bullet *bullet) {
    mEnemyRenderer->renderBullet(bullet, mTheme);
}

void RenderDriver::drawPlayer(const World *world) {
    mWorldRenderer->drawPlayer(world, mTheme);
}

void RenderDriver::startWorld() {
    // TODO: yeah...
    /*mPainter->prgDefault.bind();
    mPainter->prgDefault.setUniform("u_scroll", mGameScene->calcScrollX(), mGameScene->calcScrollY());
    mPainter->prgHazard.bind();
    mPainter->prgHazard.setUniform("u_scroll", mGameScene->calcScrollX(), mGameScene->calcScrollY());
    mPainter->prgDeadlyRail.bind();
    mPainter->prgDeadlyRail.setUniform("u_scroll", mGameScene->calcScrollX(), mGameScene->calcScrollY());
    mPainter->prgDefault.bind();*/
    mAssetRegistry->defaultProgram().bind();
    mAssetRegistry->defaultProgram().setUniform("u_scroll", mGameScene->calcScrollX(), mGameScene->calcScrollY());
}

void RenderDriver::endWorld() {
    //mPainter->prgDefault.setUniform("u_scroll", 0, 0);
    mAssetRegistry->defaultProgram().setUniform("u_scroll", 0, 0);
}

void RenderDriver::drawFinishLine(const World *world) {
    mWorldRenderer->drawFinishLine(world, mTheme);
}

void RenderDriver::drawFloor(const World *world) {
    mWorldRenderer->drawFloor(world, mTheme);
}
