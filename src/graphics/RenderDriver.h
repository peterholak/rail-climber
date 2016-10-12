#pragma once

#include "game_logic/RenderDriverInterface.h"

class RailRenderer;
class WorldRenderer;
class WallRenderer;
class WaveGeneratorRenderer;
class SmokeRenderer;
class EnemyRenderer;
class SharedAssetRegistry;
class Theme;
class GameScene;
class UnitConverter;
class BitmapRenderer;

class RenderDriver : public RenderDriverInterface {
public:
    RenderDriver(const UnitConverter &unitConverter, SharedAssetRegistry *assetRegistry, const BitmapRenderer &bitmapRenderer);

    // TODO: get rid of this shit, maybe introduce viewport concept at interface level? or separate scroll from gamescene? (no?) or...
    void setGameScene(GameScene *gameScene) { mGameScene = gameScene; }

    virtual void startWorld();
    virtual void endWorld();

    virtual void drawRail(const Rail *rail);
    virtual void drawStaticObject(const StaticObject *staticObject);
    virtual void drawWaveGenerator(const WaveGenerator *waveGenerator);
    virtual void drawGrass(const Grass *grass);
    virtual void drawEnemy(const Enemy *enemy);
    virtual void drawBullet(const Bullet *bullet);

    virtual void drawFinishLine(const World *world);
    virtual void drawPlayer(const World *world);
    virtual void drawFloor(const World *world);

    Theme *getTheme() { return mTheme; }

private:
    RailRenderer *mRailRenderer;
    WaveGeneratorRenderer *mWaveGeneratorRenderer;
    WorldRenderer *mWorldRenderer;
    WallRenderer *mWallRenderer;
    SmokeRenderer *mSmokeRenderer;
    EnemyRenderer *mEnemyRenderer;
    Theme *mTheme;
    GameScene *mGameScene;
    SharedAssetRegistry *mAssetRegistry;
    const UnitConverter &mUnitConverter;
};
