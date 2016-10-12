#pragma once

#include <string>
#include <memory>
#include <game_logic/SceneManager.h>
#include <game_logic/LevelMetadataRegistry.h>
#include <game_logic/LevelPlayStatusRegistry.h>
#include <pthread.h>

class GameScene;
class GameMenu;
class RenderDriverInterface;
class SharedAssetRegistry;
struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

class RailClimberAppEm {
public:
    RailClimberAppEm(const std::string &settingsFile);

    RailClimberAppEm(const RailClimberAppEm& orig) = delete;
    RailClimberAppEm& operator=(const RailClimberAppEm& other) = delete;
    virtual ~RailClimberAppEm();

    int run();

private:
    int mScreenWidth, mScreenHeight;
    float mGameWidth, mGameHeight;
    bool mAntialiasing;
    bool mVsync;
    int mStepDelay;
    long long lastFpsUpdate = 0;
    int frames = 0;

    GameScene *mGameScene;
    GameMenu *mGameMenu;
    SDL_Surface *mSdlScreen;
    SDL_Window *mSdlWindow;
    SDL_Renderer *mSdlRenderer;
    SharedAssetRegistry *mAssetRegistry;
    RenderDriverInterface *mRenderDriver;

    void initOpenGl();
    void cleanupOpenGl();
    void reloadOpenGl();

    void handleEvents();
    void handleKeyboardEvent(const SDL_Event &event);
    void handleMouseEvent(const SDL_Event &event);
    void tick();
    void draw();

    void moveCursorToPlayer();

    SceneManager *mSceneManager;
    std::shared_ptr<UnitConverter> mUnitConverter;

    LevelMetadataRegistry mMetadataRegistry;
    LevelPlayStatusRegistry mPlayStatusRegistry;

    void runRenderThread();
    void emscriptenLoop();

    pthread_t mRenderThread;
    static void *threadStartHelper(void *data) {
        static_cast<RailClimberApp*>(data)->runRenderThread();
        return nullptr;
    }

    static void emscriptenLoopHelper(void *data) {
        static_cast<RailClimberApp*>(data)->emscriptenLoop();
    }
};
