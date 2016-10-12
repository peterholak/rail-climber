#pragma once

#include "platform/core.h"
#include "ui/MenuScreen.h"
#include "level_loader/svg/InkscapeSvgLevelLoader.h"
#include "graphics/IFadeCallback.h"
#include "Vibrator.h"
#include "LevelMetadataRegistry.h"
#include "LevelPlayStatusRegistry.h"
#include "UnitConverter.h"
#include <string>
#include <vector>
#include <list>

class World;
class Button;
class Label;
class AnimationWidget;
class LevelMetadata;
class XmlMetadataLoader;
class SharedAssetRegistry;
class UnitConverter;

#define GAME_HEIGHT 10.0f
#define LEVELS_PER_SCREEN 15
#define TEXT_FADE_FADE 20
#define TEXT_FADE_TEXT 50
#define TEXT_FADE_QUICK 10

/**
 * Handles the game interface and its interaction with the user.
 */
class GameScene : public SceneInterface, public IFadeCallback {
public:
    GameScene(
        const UnitConverter &unitConverter,
        const SharedAssetRegistry &assetRegistry,
        const BitmapRenderer &bitmapRenderer,
        int screenWidth,
        int screenHeight,
        float gameWidth,
        float gameHeight,
        const LevelMetadataRegistry &metadataRegistry,
        LevelPlayStatusRegistry &playStatusRegistry,
        RenderDriverInterface *renderDriver
    );
    ~GameScene();

    void initialize();
    void cleanup();
    void updateGame(int stepDelay);
    void pauseGame(bool pause);

    void toggleSound();

    void showOverlayMenu(const std::string &screen);
    void hideOverlayMenu();

    void finishLevel();

    void fadeCallback(const std::map<std::string, std::string> &callbackMap, bool cancelled);

    World *getWorld() {
        return mWorld;
    }

    std::string getLastLevel() {
        return mLastLevel;
    }

    float lastX = 0, lastY = 0;
    bool dragging = false;
    bool inMenu = false, overlayMenu = false;
    bool showFPS = false, showBackground = true, compressTextures = false;
    bool paused = false;

    long long lastTicks = 0;
    long long accumulator = 0, menuAccumulator = 0;

    void initSound();


    float calcScrollX();
    float calcScrollY();
    Vec2 calcScrollXY();

    virtual bool handleTouchPress(const Vec2& inGameXY);
    virtual bool handleTouchMove(const Vec2& inGameXY);
    virtual bool handleTouchRelease(const Vec2& inGameXY);
    virtual bool handleBackAction();
    virtual void render();
    virtual const char* getSceneId() const { return "Game"; }


private:
    float mScreenW, mScreenH, mRatio = 1.0f, mGameWidth = 10.0f;
    World *mWorld = nullptr;
    std::string mLastLevel;
    bool mShowTips = true;
    int mVolumeTimer = 0;
    unsigned int mLevelScreen = 0, mNumLevelScreens = 1;

    Label *mLblTipText = nullptr;
    Label *mLblTipImage = nullptr;
    AnimationWidget *mAnimation = nullptr;
    MenuScreen *mTipScreen = nullptr;
    const SharedAssetRegistry &mAssetRegistry;
    const BitmapRenderer &mBitmapRenderer;

    const LevelMetadata *mCurrentLevel = nullptr;
    LevelPlayStatus mCurrentLevelStatus;

    void removeChainLayer(const std::map<std::string, std::string> &in, std::map<std::string, std::string> &out);

    void showTips(const LevelMetadata *levelMetadata);
    void step(float t);
    void setAchievementMenu();

    void handleVersionUpgrade();

    void onPlayerDeath();
    World *loadLevelFromFile(const std::string &filename, int version);

    const UnitConverter &mUnitConverter;
    LevelLoaderInterface *mLevelLoader = nullptr;
    Vibrator mVibrator;

    // TODO: change this stuff to pointers?
    const LevelMetadataRegistry &mMetadataRegistry;
    LevelPlayStatusRegistry &mPlayStatusRegistry;

    RenderDriverInterface *mRenderDriver;

    void drawBackground(Theme *theme);

public:
    static unsigned int levelsPerScreen;
    float scrollX = 0, scrollY = 0;

    void cleanupWorld();

    void playLevel(int levelNum);

    void restartLevel();

    void playNextLevel();

    static constexpr const char* pauseButtonTextureId = "pause-button";
    static constexpr const char* gravityBoostTextureId = "gravity-boost-button";

    void drawInterface();
};
