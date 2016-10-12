#include "RailClimberApp.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include "game_logic/SceneManager.h"
#include "Settings.h"
#include "Logging.h"
#include "game_logic/GameScene.h"
#include "game_logic/UnitConverter.h"
#include "ui/GameMenu.h"
#include "graphics/RenderDriver.h"
#include "graphics/SharedAssetRegistry.h"
#include "graphics/BitmapRenderer.h"
#include "Translation.h"
#include "ui/Label.h"

using namespace std;

#define MULTIPLE_THREADS false

void RailClimberApp::initOpenGl() {
    SDL_Init(SDL_INIT_VIDEO);

    if (mAntialiasing) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    }

    mSdlWindow = SDL_CreateWindow(
        "RailClimber",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        mScreenWidth,
        mScreenHeight,
        SDL_WINDOW_OPENGL
    );
    RailClimberPlatform::g_sdlWindow = mSdlWindow;
    mSdlRenderer = SDL_CreateRenderer(mSdlWindow, -1, mVsync ? SDL_RENDERER_PRESENTVSYNC : 0);
    SDL_GL_CreateContext(mSdlWindow);

//    if (!glewInited) {
        glewInit();
//        glewInited = true;
//    }
}

void RailClimberApp::cleanupOpenGl() {
    SDL_Quit();
}

RailClimberApp::RailClimberApp(const string &settingsFile) : mSdlScreen(nullptr), mPlayStatusRegistry(mMetadataRegistry) {
    Log::info("RailClimber started");
    RailClimberPlatform::initialize();

    if (!Settings::load("railclimber.xml")) {
        throw std::runtime_error("Couldn't load settings file, exiting...");
    }

    Settings::loadSaved("RailClimber");

    mScreenWidth = Settings::getInt("Graphics/Width");
    mScreenHeight = Settings::getInt("Graphics/Height");

    float aspectRatio = (float)mScreenWidth / mScreenHeight;
    mGameHeight = GAME_HEIGHT;
    mGameWidth = aspectRatio * mGameHeight;

    mAntialiasing = Settings::getBool("Graphics/Antialiasing");
    mVsync = Settings::getBool("Graphics/VSync");

    mStepDelay = Settings::getInt("World/StepDelay");

    initOpenGl();
    TTF_Init();

    Tr::initialize();

    UnitConverter *unitConverter = new UnitConverter(mScreenWidth, mScreenHeight, mGameWidth, mGameHeight);

    mMetadataRegistry.loadMetadataForLevels();
    mPlayStatusRegistry.loadAllPlayStatuses();

    mSceneManager = new SceneManager(*unitConverter);

    mAssetRegistry = new SharedAssetRegistry(*unitConverter);
    mBitmapRenderer = new BitmapRenderer(mAssetRegistry->defaultProgram());
    mRenderDriver = new RenderDriver(*unitConverter, mAssetRegistry, *mBitmapRenderer);
    mGameScene = new GameScene(*unitConverter, *mAssetRegistry, *mBitmapRenderer, mScreenWidth, mScreenHeight, mGameWidth, mGameHeight, mMetadataRegistry, mPlayStatusRegistry, mRenderDriver);
    // TODO: ...
    static_cast<RenderDriver*>(mRenderDriver)->setGameScene(mGameScene);

    mGameScene->initialize();
    mSceneManager->registerScene(mGameScene);
/*
    Painter *temporaryPainter = new Painter(unitConverter);
    temporaryPainter->initialize(mScreenWidth, mScreenHeight, 1.0f);
    temporaryPainter->createGraphics();
*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.59f, 0.72f, 0.9f, 1.0f);

    mGameMenu = new GameMenu(*unitConverter, *mAssetRegistry, *mBitmapRenderer, mMetadataRegistry, mPlayStatusRegistry, mGameScene);
    mGameMenu->registerWithSceneManager(mSceneManager);

    mSceneManager->pushScene(mGameMenu->mainMenu);

    if (MULTIPLE_THREADS) {
        pthread_create(&mRenderThread, nullptr, RailClimberApp::threadStartHelper, this);
    }

/*
    sRatio = (float) screenW / screenH;
    sRatio10 = sRatio * 10;

    seconds = time(nullptr);
    frames = 0;
    updateFps = false;
*/
}

RailClimberApp::~RailClimberApp() {

}

int RailClimberApp::run() {
    while (!RailClimberPlatform::g_quit) {
        handleEvents();
        tick();

        if (!MULTIPLE_THREADS) {
            runRenderThread();
        }
    }

    if (MULTIPLE_THREADS) {
        pthread_join(mRenderThread, nullptr);
    }

    //mGameScene->cleanup();
    Settings::unloadSaved();
    TTF_Quit();
    cleanupOpenGl();
    return 0;
}

void RailClimberApp::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            RailClimberPlatform::quit();
        }

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            //handleKeyboardEvent(event);
        }

        if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION) {
            handleMouseEvent(event);
        }
    }
}

void RailClimberApp::handleKeyboardEvent(const SDL_Event &event) {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
        RailClimberPlatform::quit();
        return;
    }

    if (event.type != SDL_KEYUP) {
        return;
    }

    switch (event.key.keysym.sym) {
        case SDLK_BACKSPACE:
            mSceneManager->handleBackAction();
            break;

        case SDLK_g:
            mGameScene->getWorld()->setGravityBoost(true);
            break;

        case SDLK_a:
            //mPainter->aaOverdraw = !mPainter->aaOverdraw;
            break;

        case SDLK_p:
            mGameScene->pauseGame(1);
            break;

        case SDLK_u:
            mGameScene->pauseGame(0);
            break;

        case SDLK_f:
            mGameScene->finishLevel();
            break;

        case SDLK_d:
            Log::debug("player at %.3f %.3f", mGameScene->getWorld()->getPlayer().pos.x(), mGameScene->getWorld()->getPlayer().pos.y());
            break;

        case SDLK_r:
            reloadOpenGl();
            break;

        case SDLK_UP:
            //SDL_RemoveTimer(mainTimer);
            //speed *= 2;
            //mainTimer = SDL_AddTimer(speed, timerProc, nullptr);
            break;

        case SDLK_DOWN:
            //SDL_RemoveTimer(mainTimer);
            //speed /= 2;
            //if (speed == 0) {
//                speed = 1;
//            }
            //mainTimer = SDL_AddTimer(speed, timerProc, nullptr);
            break;

        case SDLK_w:
            moveCursorToPlayer();
            break;

        case SDLK_t:
            //const_cast<MenuScreen*> (game->getMenu())->manualTick();
            break;

        case SDLK_q:
            //World *w = GameScene::getInstance()->getWorld();
            //w->unloadLevel();
            //w->loadLevelFromFile(GameScene::getInstance()->getLastLevel());
            break;

        default:
            break;
    }
}

void RailClimberApp::handleMouseEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEMOTION) {
        mSceneManager->handleTouchMove(event.motion.x, event.motion.y);
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        mSceneManager->handleTouchRelease(event.button.x, event.button.y);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            mSceneManager->handleTouchPress(event.button.x, event.button.y);
        }else if (event.button.button == SDL_BUTTON_RIGHT) {
            moveCursorToPlayer();
        }
    }
}

void RailClimberApp::tick() {
    /*if (time(nullptr) - seconds >= 1) {
        updateFps = true;
    }*/
    mGameScene->updateGame(20);
}

void RailClimberApp::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    //mPainter->setScroll(mGameScene);
    mSceneManager->renderScene();

    SDL_GL_SwapWindow(mSdlWindow);
}

void RailClimberApp::moveCursorToPlayer() {
    World *world = mGameScene->getWorld();

    const World::Player& player = world->getPlayer();
    float xShift = 0, yShift = 0;
/*
    if (mPainter->scrollX > 0 && mPainter->scrollX - (world->levelWidth - mGameWidth) < -0.01) {
        xShift = -player.pos.x() + (mGameWidth / 2);
    } else if (mPainter->scrollX > 0) {
        xShift = -world->levelWidth + mGameWidth;
    }

    if (mPainter->scrollY > 0) {
        yShift = -player.pos.y() + mGameHeight / 2;
    }
*/
    SDL_WarpMouseInWindow(
        mSdlWindow,
        (world->getPlayer().pos.x() + xShift) * mScreenWidth / mGameWidth,
        mScreenHeight - (world->getPlayer().pos.y() + yShift) * mScreenHeight / mGameHeight
    );
}

void RailClimberApp::reloadOpenGl() {
//    mGameScene->destroyGUI();
    mGameScene->getWorld()->destroyBufferObjects();
    TTF_Quit();

    cleanupOpenGl();
    initOpenGl();

    TTF_Init();
    // TODO: reload GUI
}

void RailClimberApp::runRenderThread() {
//    while (!RailClimberPlatform::g_quit) {
        draw();

        if (RailClimberPlatform::getTicks() - lastFpsUpdate >= 1000) {
            //mPainter->updateTimeTextures((float) frames*1000 / (RailClimberPlatform::getTicks() - lastFpsUpdate), 0, true);
            frames = 0;
            lastFpsUpdate = RailClimberPlatform::getTicks();
        }
        frames++;
//    }
}
