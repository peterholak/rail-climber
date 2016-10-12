#pragma once

struct SDL_Window;
struct SDL_Renderer;
class UnitConverter;
class SharedAssetRegistry;
class BitmapRenderer;
class SandboxCase;

class RailClimberSandbox {

public:
    RailClimberSandbox();
    int run();
    template<class T> void setSandboxCase() {
        mSandboxCase = new T(*mUnitConverter, *mSharedAssets);
    }

private:
    void setupOpenGl();

    SDL_Window *mSdlWindow;
    SDL_Renderer *mSdlRenderer;

    void initialize();
    void handleEvents();
    void draw();

    UnitConverter *mUnitConverter;
    SharedAssetRegistry *mSharedAssets;
    BitmapRenderer *mBitmapRenderer;
    SandboxCase *mSandboxCase = nullptr;
};


