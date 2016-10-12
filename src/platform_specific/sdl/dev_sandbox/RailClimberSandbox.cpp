#include "RailClimberSandbox.h"
#include <SDL_video.h>
#include <SDL.h>
#include "platform_specific/sdl/sdl_core.h"
#include <GL/glew.h>
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"
#include "graphics/BitmapRenderer.h"
#include "ui/Label.h"
#include "platform/core.h"
#include <SDL_ttf.h>
#include "SandboxCase.h"

using namespace std;

int RailClimberSandbox::run() {
    if (mSandboxCase != nullptr) {
        mSandboxCase->initializeCase();
    }

    while (!RailClimberPlatform::g_quit) {
        handleEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        draw();
        SDL_GL_SwapWindow(mSdlWindow);
    }
    return 0;
}

RailClimberSandbox::RailClimberSandbox() {
    RailClimberPlatform::initialize();
    setupOpenGl();
    TTF_Init();
    initialize();
}

void RailClimberSandbox::setupOpenGl() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    mSdlWindow = SDL_CreateWindow(
        "RailClimber",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        800,
        SDL_WINDOW_OPENGL
    );
    RailClimberPlatform::g_sdlWindow = mSdlWindow;
    mSdlRenderer = SDL_CreateRenderer(mSdlWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_GL_CreateContext(mSdlWindow);

    glViewport(0, 0, 800, 800);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.59f, 0.72f, 0.9f, 1.0f);

    glewInit();
}

void RailClimberSandbox::initialize() {
    mUnitConverter = new UnitConverter(800, 800, 10.0f, 10.0f);
    mSharedAssets = new SharedAssetRegistry(*mUnitConverter);
    mBitmapRenderer = new BitmapRenderer(mSharedAssets->defaultProgram());


}

void RailClimberSandbox::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            RailClimberPlatform::g_quit = true;
        }

        if (mSandboxCase != nullptr) {
            mSandboxCase->handleCaseEvent(event);
        }
    }
}

void RailClimberSandbox::draw() {
    if (mSandboxCase != nullptr) {
        mSandboxCase->drawCase(*mBitmapRenderer);
    }
}
