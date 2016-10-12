#include "graphics/RenderDriver.h"
#include "GameScene.h"

#include "game_logic/UnitConverter.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/AnimationWidget.h"
#include "Settings.h"
#include "Sound.h"
#include "platform/sound.h"
#include "Translation.h"
#include "Logging.h"
#include "level_loader/XmlMetadataLoader.h"
#include "graphics/SharedAssetRegistry.h"
#include "graphics/Theme.h"
#include "graphics/ShaderProgram.h"

#define RECTANGLE3(x,y,w,h) {x,y,0,x+(w),y,0,x,y+(h),0,x+(w),y+(h),0}
#define RGBA4(r,g,b,a) {r,g,b,a,r,g,b,a,r,g,b,a,r,g,b,a}
#define SETFIELDIN4x4(a,i,v) {a[(i)] = a[(i)+4] = a[(i)+8] = a[(i)+12] = v;}
#define RECTANGLE_TEXCOORD(w,h) {0.0f,0.0f,w,0.0f,0.0f,h,w,h}

using namespace std;

unsigned int GameScene::levelsPerScreen = LEVELS_PER_SCREEN;

GameScene::GameScene(
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
) : mScreenW(screenWidth), mScreenH(screenHeight), mMetadataRegistry(metadataRegistry), mPlayStatusRegistry(playStatusRegistry), mUnitConverter(unitConverter), mRenderDriver(renderDriver), mBitmapRenderer(bitmapRenderer), mAssetRegistry(assetRegistry)
{
    mVibrator.vibrateEnabled = Settings::getSavedBool("settings", "vibrate", true);
    mLevelLoader = new InkscapeSvgLevelLoader();
}

GameScene::~GameScene() {

}

void GameScene::cleanup() {
    if (mWorld) {
        mWorld->destroyBufferObjects();
    }
    Sound::getInstance()->cleanup();
    cleanupWorld();
}

void GameScene::initialize() {
    mRatio = mScreenW / mScreenH;
    mGameWidth = mRatio * 10.0f;
    Log::debug("Setting inMenu to true 1");
    inMenu = true;
    overlayMenu = false;
    mShowTips = Settings::getSavedBool("settings", "tips", true);
    showFPS = Settings::getBool("Graphics/ShowFPS");
    showBackground = true;
    compressTextures = true;
    lastTicks = RailClimberPlatform::getTicks();
    accumulator = 0;
    menuAccumulator = 0;

    mLevelScreen = 0;
    mNumLevelScreens = (mMetadataRegistry.getLevelCount() / levelsPerScreen) + (mMetadataRegistry.getLevelCount() % levelsPerScreen ? 1 : 0);

    handleVersionUpgrade();

    initSound();
    /*int volume = Settings::getSavedInt("sound", "volume", 100);
    RailClimberPlatform::setVolume(volume);*/

///    mPainter->fadeIn(TEXT_FADE_FADE);
}

void GameScene::initSound() {
    Sound *sound = Sound::getInstance();
    sound->initialize();
    sound->enabled = Settings::getSavedBool("settings", "sound", true);
    sound->loadSound("railAttach", "sounds/attach.wav");
    sound->loadSound("enemyHit", "sounds/enemy.wav");
    sound->loadSound("groundHit", "sounds/crash.wav");
    sound->loadSound("death", "sounds/dead.wav");
    sound->loadSound("victory", "sounds/victory.wav");
    sound->loadSound("waveGenerator", "sounds/wavegen.wav");
    sound->loadSound("menu", "sounds/menu.wav");
}

void GameScene::finishLevel() {
    Sound::getInstance()->playSound("victory");

    showOverlayMenu("Finished");

    int seconds = mWorld->levelTimer / 1000;
    int minutes = seconds / 60;
    int centi = (mWorld->levelTimer - seconds * 1000) / (1000 / 100);
    seconds -= minutes * 60;
    char lblStr[16];
    snprintf(lblStr, 15, "%d:%.2d.%.2d", minutes, seconds, centi);
    /*mGameMenu->mTimeLabel->setText(lblStr);

    mGameMenu->mLblMedal->changeImage(&mPainter->texMedal[3]);
    for (int i = 0; i < 3; i++) {
        if (mWorld->levelTimer < (mCurrentLevel->medalTimes[i]*1000)) {
            mGameMenu->mLblMedal->changeImage(&mPainter->texMedal[i]);
            break;
        }
    }*/

    mCurrentLevelStatus.passed = true;

    if (mCurrentLevelStatus.highScore == LevelPlayStatus::NO_HIGH_SCORE || mWorld->levelTimer < mCurrentLevelStatus.highScore) {
        mCurrentLevelStatus.highScore = mWorld->levelTimer;
    }

    mPlayStatusRegistry.storeLevelPlayStatus(mCurrentLevel->number, mCurrentLevelStatus);

    int highestMedal = mPlayStatusRegistry.getReachedMedal(mCurrentLevel->number);
    int medalTime;
    if (highestMedal) {
        medalTime = int(mCurrentLevel->medalTimes[highestMedal - 1] * 1000);
    }else{
        medalTime = mCurrentLevelStatus.highScore;
    }

//    mGameMenu->setupLevelButtons(mMetadataRegistry, mPlayStatusRegistry, mPainter, levelsPerScreen, mLevelScreen, mNumLevelScreens);

    string hs;
    if (highestMedal == 0) hs = _("High score");
    else if (highestMedal == 1) hs = _("Gold");
    else if (highestMedal == 2) hs = _("Silver");
    else if (highestMedal == 3) hs = _("Bronze");
    seconds = medalTime / 1000;
    minutes = seconds / 60;
    centi = (medalTime - seconds * 1000) / (1000 / 100);
    seconds -= minutes * 60;
    snprintf(lblStr, 15, "%d:%.2d.%.2d", minutes, seconds, centi);
    hs = hs + ": " + lblStr;
    //mGameMenu->mNextMedalLabel->setText(hs);
}

bool GameScene::handleTouchPress(const Vec2& inGameXY) {
    if (paused) {
        return false;
    }

///    if (mPainter->isFading()) {
///        mPainter->cancelFade();
///        return false;
///    }

    if (inMenu || overlayMenu) {
        //mGameMenu->handleTouchPress(inGameXY);
    } else {
        const World::Player &player = mWorld->getPlayer();
        float rx (inGameXY.x() + calcScrollX()), ry = (inGameXY.y() + calcScrollY());
        float rxns = inGameXY.x(), ryns = inGameXY.y();

        if (mWorld->preProcessTouch(rx, ry)) {
            //don't need to do anything, just make sure those blocks below don't execute
        } else if (rx > player.pos.x() - World::playerTouchRadius && rx < player.pos.x() + World::playerTouchRadius &&
            ry > player.pos.y() - World::playerTouchRadius && ry < player.pos.y() + World::playerTouchRadius) {
            dragging = true;
            lastX = inGameXY.x();
            lastY = inGameXY.y();
        } else if (rxns > mGameWidth - 1 && ryns < 1) {
            handleBackAction();
        } else if (rxns > mGameWidth - 1 && ryns > 9) {
            mWorld->setGravityBoost(true);
        } else {
            mWorld->touchPress(rx, ry);
        }
    }
    return true;
}

bool GameScene::handleTouchMove(const Vec2& inGameXY) {
    if (paused) {
        return true;
    }

    if (inMenu || overlayMenu) {
        //mGameMenu->handleTouchMove(inGameXY);
    } else {
        if (dragging) {
            lastX = inGameXY.x();
            lastY = inGameXY.y();
        }
    }
    return false;
}

bool GameScene::handleTouchRelease(const Vec2& inGameXY) {
    if (paused) {
        return false;
    }

    if (inMenu || overlayMenu) {
        //mGameMenu->handleTouchRelease(inGameXY);
    } else {
        if (dragging) {
            const World::Player &player = mWorld->getPlayer();
            if (player.canFire()) {
                float fx = ((inGameXY.x() + calcScrollX()) - player.pos.x()) * 100.0f;
                float fy = ((inGameXY.y() + calcScrollY()) - player.pos.y()) * 100.0f;
                mWorld->firePlayer(fx, fy);
            }
            dragging = false;
        }
    }
    return true;
}

bool GameScene::handleBackAction() {
    if (overlayMenu) {
        // TODO: this will actualy happen in the overlay menu screen class...
        hideOverlayMenu();
        //mGameMenu->setActiveScreen("MainMenu");
        cleanupWorld();
        Log::debug("Setting inMenu to true 2");
        inMenu = true;
    } else {
        showOverlayMenu("Paused");
    }
    return true;
}

void GameScene::fadeCallback(const map<string, string> &callbackMap, bool cancelled) {
    if (callbackMap.empty()) {
        return;
    }

    string command = callbackMap.find("command")->second;

    if (command == "quit") {
///        mPainter->quitting = true;
        RailClimberPlatform::quit();

    } else if (command == "chain-in") {
        map<string, string> newMap;
        removeChainLayer(callbackMap, newMap);

        int newDuration = atoi(callbackMap.find("duration")->second.c_str());

///        mPainter->fadeIn(newDuration, newMap);

    } else if (command == "chain-out") {
        map<string, string> newMap;
        removeChainLayer(callbackMap, newMap);

        int newDuration = atoi(callbackMap.find("duration")->second.c_str());

///        mPainter->fadeOut(newDuration, newMap);

    } else if (command == "play-level") {
        const string &btnName = callbackMap.find("button-name")->second;
        string str(btnName, 2, btnName.length() - 2);
        int levelNum = strtol(str.c_str(), nullptr, 0) - 1;
        levelNum += mLevelScreen * levelsPerScreen;
        playLevel(levelNum);

    } else if (command == "restart") {
        restartLevel();

    } else if (command == "next-level") {
        playNextLevel();

    } else if (command == "back-to-menu") {
        cleanupWorld();
//        mGameMenu->setActiveScreen("MainMenu");
        hideOverlayMenu();
        Log::debug("Setting inMenu to true 3");
        inMenu = true;

        if (!cancelled) {
///            mPainter->fadeIn(TEXT_FADE_QUICK);
        }
    }
}

void GameScene::playNextLevel() {
    if (mCurrentLevel->number == mMetadataRegistry.getLevelCount() - 1) {
        // TODO: decide whether to handle this here on in FinishedMenu
//            mGameMenu->setActiveScreen("MoreLevels");
    } else if ((mCurrentLevel->number + 1) % levelsPerScreen == 0) {
        mLevelScreen++;
//                mGameMenu->setupLevelButtons(mMetadataRegistry, mPlayStatusRegistry, mPainter, levelsPerScreen, mLevelScreen, mNumLevelScreens);
    }

    mCurrentLevel = mMetadataRegistry.getMetadataForLevel(mCurrentLevel->number + 1);
    mCurrentLevelStatus = mPlayStatusRegistry.getLevelPlayStatus(mCurrentLevel->number + 1);
    mLastLevel = string("levels/") + mCurrentLevel->filename;

    cleanupWorld();
    mWorld = loadLevelFromFile(mLastLevel, mCurrentLevel->version);
    if (mCurrentLevel->hasTip) {
            showTips(mCurrentLevel);
        } else {
            hideOverlayMenu();
        }

///    mPainter->activeBkg = mCurrentLevel->background;
///    mPainter->activeTheme = mCurrentLevel->theme;
}

void GameScene::restartLevel() {
    cleanupWorld();
    mWorld = loadLevelFromFile(mLastLevel, mCurrentLevel->version);
    hideOverlayMenu();
}

void GameScene::playLevel(int levelNum) {
    mCurrentLevel = mMetadataRegistry.getMetadataForLevel(levelNum);
    mCurrentLevelStatus = mPlayStatusRegistry.getLevelPlayStatus(levelNum);
    mLastLevel = string("levels/") + mCurrentLevel->filename;
    cleanupWorld();
    mWorld = loadLevelFromFile(mLastLevel, mCurrentLevel->version);
    if (mCurrentLevel->hasTip) {
        showTips(mCurrentLevel);
    }
    inMenu = false;

///    mPainter->activeBkg = mCurrentLevel->background;
///    mPainter->activeTheme = mCurrentLevel->theme;
}

void GameScene::removeChainLayer(const map<string, string> &inChain, map<string, string> &out) {
    for (auto it = inChain.cbegin(); it != inChain.cend(); ++it) {
        //remove prefix and add to new map
        if (it->first.find("chain-") == 0) {
            out[it->first.substr(6)] = it->second;
        }
    }
}

void GameScene::showTips(const LevelMetadata *levelMetadata) {
/*    if (mShowTips) {
        if (mAnimation) {
            mTipScreen->widgets.remove(mAnimation);
            delete mAnimation;
            mAnimation = nullptr;
        }
        if (levelMetadata->tipType == LevelMetadata::PictureTip) {
            mLblTipImage->changeImage(levelMetadata->tipData);
            mLblTipImage->visible = true;
            mTipLabel->visible = false;
            Button *button = static_cast<Button*> (mGameMenu->getScreen("Tips")->findWidget("TipsHide"));
            button->setEnabled(true);
            button->setText("Ok");
        } else if (levelMetadata->tipType == LevelMetadata::TextTip) {
            mTipLabel->setText(levelMetadata->tipData);
            mTipLabel->visible = true;
            mLblTipImage->visible = false;
            Button *button = static_cast<Button*> (mGameMenu->getScreen("Tips")->findWidget("TipsHide"));
            button->setEnabled(true);
            button->setText("Ok");
        } else {
            mTipLabel->visible = false;
            mLblTipImage->visible = false;
            mAnimation = AnimationWidget::getAnimationForLevel(mPainter, mGameMenu, levelMetadata->number + 1, 0.75, 7.5, mGameWidth - 1.5f, 4);
            mGameMenu->addWidget("Tips", mAnimation);
        }
        showOverlayMenu("Tips");
    } else {
        hideOverlayMenu();
    }*/
}

void GameScene::step(float t) {
///    Painter *painter = mPainter;
    if (!inMenu/* && !painter->isFading()*/) {
/*        painter->hazardTime += 0.008;

        painter->hazardFrameTicks++;
        if (painter->hazardFrameTicks >= 2) {
            painter->hazardFrameTicks = 0;
            if (painter->hazardFrame >= NOISE_FRAMES - 1) {
                painter->hazardFrame = 0;
            }else{
                painter->hazardFrame++;
            }
        }*/
        mWorld->step(t);
    }
}

void GameScene::updateGame(int stepDelay) {
    long long ticks = RailClimberPlatform::getTicks();
    long long gameTicks = ticks - lastTicks;
    lastTicks = ticks;

    //volumeIndicatorStep();

///    Painter *painter = mPainter;
    if (!paused) {
        if (!inMenu && !overlayMenu) {
            accumulator += gameTicks;
        }else{
            menuAccumulator += gameTicks;
        }
    }

    if (inMenu || overlayMenu) {
        menuAccumulator = min<long long>(menuAccumulator, 5 * stepDelay);
        while (menuAccumulator >= stepDelay) {
            menuAccumulator -= stepDelay;
            //mGameMenu->tick();
/*            if (painter->isFading()) {
                painter->fadeStep();
            }*/
        }
    } else {
        accumulator = min<long long>(accumulator, 5 * stepDelay);
        while (accumulator >= stepDelay) {
            accumulator -= stepDelay;
            step(0.016f);
/*            if (painter->isFading()) {
                painter->fadeStep();
            }*/
        }
    }
}

void GameScene::showOverlayMenu(const string &screen) {
    dragging = false;
    mWorld->pause(true);
    sceneChangeRequested(screen.c_str(), SceneInterface::Push);
//    mGameMenu->setActiveScreen(screen);
}

void GameScene::hideOverlayMenu() {
    overlayMenu = false;
    if (mWorld) {
        mWorld->pause(false);
    }
}

void GameScene::pauseGame(bool pause) {
    //Sound *sound = Sound::getInstance();
    if (pause) {
        paused = true;
        //sound->enabled = false;
        if (!inMenu && !overlayMenu) {
            showOverlayMenu("Paused");
        }
        //Unload textures
    } else {
        paused = false;
        lastTicks = RailClimberPlatform::getTicks();
        //sound->enabled = Settings::getSavedBool("settings", "sound", true);
        //Reload textures
    }
}

void GameScene::handleVersionUpgrade() {
    int currentVersion = Settings::getInt("Game/Version");
    int previousVersion = Settings::getSavedInt("game", "version", 0);
    if (previousVersion == 0) { // first-time run

    } else if (previousVersion < currentVersion) { //first run after upgrade

    } else if (previousVersion > currentVersion) { //first run after downgrade

    }
    Settings::setSavedInt("game", "version", currentVersion);
}

void GameScene::setAchievementMenu() {
    //AchievementManager *am = AchievementManager::getInstance();
}

float GameScene::calcScrollX() {
    return min(
        max(0.0f, mWorld->getPlayer().pos.x() - mGameWidth / 2.0f),
        mWorld->levelWidth - mGameWidth
    );
}

float GameScene::calcScrollY() {
    return max(0.0f, mWorld->getPlayer().pos.y() - GAME_HEIGHT/2.0f);
}

Vec2 GameScene::calcScrollXY() {
    return Vec2(calcScrollX(), calcScrollY());
}

void GameScene::onPlayerDeath() {
    showOverlayMenu("Dead");
}

World *GameScene::loadLevelFromFile(const std::string &filename, int version) {
    char *levelStr = RailClimberPlatform::loadFile(filename.c_str());

    if (!levelStr) {
        Log::error("Error opening level file %s", filename.c_str());
        return nullptr;
    }

    string data = levelStr;
    delete[] levelStr;

    World *world = mLevelLoader->loadLevel(data, version);
    world->levelFinished.connect<GameScene, &GameScene::finishLevel>(this);
    world->playerDied.connect<GameScene, &GameScene::onPlayerDeath>(this);
    return world;
}

void GameScene::cleanupWorld() {
    delete mWorld;
    mWorld = nullptr;
}

void GameScene::render() {
    // TODO
    Theme *theme = static_cast<RenderDriver*>(mRenderDriver)->getTheme();

    drawBackground(theme);
    mWorld->render(mRenderDriver);
    drawInterface();
}

void GameScene::drawBackground(Theme *theme) {
    const float parallaxSpeed = 0.675f;
    const float parallaxRest = 1.0f - parallaxSpeed;
    GLfloat bkgScrollX = scrollX * parallaxSpeed;

    GLfloat bkgEdge = 10.0f;
    GLfloat bkgEdgePlus = bkgEdge + scrollY * parallaxRest;

    GLfloat bkgTexW = mRatio * 2.0f;
    GLfloat bkgTexX = (bkgScrollX / mGameWidth) * bkgTexW;
    GLfloat bkgTopTexX = bkgTexX;
    GLfloat bkgTopTexW = bkgTexW;

    GLfloat bkgTopTexY = max(0.0f, scrollY * parallaxSpeed - 10.0f) / (10.0f * parallaxSpeed);
    GLfloat bkgTopTexH = 2.0f;

    GLfloat color[] = RGBA4(1, 1, 1, 1);

    const ShaderProgram& defaultProgram = mAssetRegistry.defaultProgram();
    defaultProgram.setColor(color);
    defaultProgram.setUniform("u_scroll", scrollX, scrollY);
    if (showBackground) {
        glDisable(GL_BLEND);

        if (scrollY < bkgEdgePlus) {
            GLfloat bkg[] = RECTANGLE3(scrollX, scrollY * parallaxRest, mGameWidth, bkgEdge);
            GLfloat bkgTex[] = {bkgTexX, 0, bkgTexX + bkgTexW, 0, bkgTexX, 1, bkgTexX + bkgTexW, 1};

            defaultProgram.setPosition(bkg);
            defaultProgram.setTextureCoordinates(bkgTex);
            theme->getTexture("background").bind();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        GLfloat topbkg[] = RECTANGLE3(scrollX, max(scrollY, bkgEdgePlus), mGameWidth, 10.0f);
        GLfloat topbkgTex[] = {bkgTopTexX, bkgTopTexY, bkgTopTexX + bkgTopTexW, bkgTopTexY, bkgTopTexX, bkgTopTexY + bkgTopTexH, bkgTopTexX + bkgTopTexW, bkgTopTexY + bkgTopTexH};
        theme->getTexture("background-top").bind();
        defaultProgram.setPosition(topbkg);
        defaultProgram.setTextureCoordinates(topbkgTex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glEnable(GL_BLEND);
    }
}

void GameScene::drawInterface() {
    const World *w = getWorld();

    if (dragging) {
        const World::Player &player = const_cast<World*> (w)->getPlayer();
        float x = lastX, y = lastY;
        Vec2 r1, r2, r3, r4;
        Vec2 force = Vec2(x, y) - (player.pos - calcScrollXY());
        float len = force.length();
        if (len > World::maximumForce) {
            force = force.toDistance(World::maximumForce);
        }

        if (force.length() >= World::playerRadius + 0.25f) {
            Vec2 start = player.pos - calcScrollXY() + force.toDistance(World::playerRadius + 0.1f);
            Vec2 mid = player.pos - calcScrollXY() + force.toDistance(World::playerRadius + 0.25f + len * 0.06f);
            Vec2 end = player.pos - calcScrollXY() + force;

            Vec2::boxAroundLine(0.35f, start, end, r1, r2, r3, r4);
            GLfloat coords[] = {
                r1.x(), r1.y(), 0,
                end.x(), end.y(), 0,
                mid.x(), mid.y(), 0,

                r3.x(), r3.y(), 0,
                end.x(), end.y(), 0,
                mid.x(), mid.y(), 0
            };
            bool fire = player.canFire();
            float e = fire ? 1 : 0.5;
            float a = fire ? 0.3 : 0.1;
            GLfloat color[] = {e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a};

            const ShaderProgram &prgDefault = mAssetRegistry.defaultProgram();
            prgDefault.bind();
            prgDefault.resetDefaultAttributes();
            prgDefault.setUniform("noTexture", 1);
            prgDefault.setPosition(coords);
            prgDefault.setColor(color);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            prgDefault.setUniform("noTexture", 0);
        }
    }

    GLfloat color[] = RGBA4(1, 1, 1, 1);
    if (w->gravityBoost) {
        color[0] = color[4] = color[8] = color[12] = 0.5;
        color[1] = color[5] = color[9] = color[13] = 1;
        color[2] = color[6] = color[10] = color[14] = 0.5;
    }

    mBitmapRenderer.renderTexture(mAssetRegistry.getUiTexture("gravity-boost"), mUnitConverter.gameWidth() - 1.0f, mUnitConverter.gameHeight() - 1.0f, 1.0f, 1.0f);
    mBitmapRenderer.renderTexture(mAssetRegistry.getUiTexture("pause"), mUnitConverter.gameWidth() - 1.0f, 0, 1.0f, 1.0f);
    //drawTextureRect(texTime, 0.2f, 9.8f, false, ((float) texTime.width / texTime.height)*0.6f, 0.6f);
}
