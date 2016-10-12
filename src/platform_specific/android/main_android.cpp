#include <jni.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <assert.h>
#include <android/log.h>
#include "platform/core.h"
#include "graphics/Painter.h"
#include "Sound.h"
#include "game_logic/Game.h"
#include "Settings.h"
#include "Translation.h"
#include "Logging.h"
#include "ui/Menu.h"

Painter *painter = nullptr;
Game *game = nullptr;
bool finished = false;

using namespace RailClimberPlatform;

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_init(JNIEnv *env, jobject obj, jint w, jint h);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_createGraphics(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_cleanup(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_draw(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_update(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_pause(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_resume(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_touchPress(JNIEnv *env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_touchMove(JNIEnv *env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_touchRelease(JNIEnv *env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_backButtonPressed(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_setFPS(JNIEnv *env, jobject obj, jfloat fps);
    JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_requestDebugAction(JNIEnv *env, jobject obj);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    g_jvm = jvm;
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_init(JNIEnv *env, jobject obj, jint w, jint h) {
    Log::debug("Init started - NDK, thread %d", gettid());

    int screenW = w, screenH = h;
    finished = false;

    if (!Settings::load("railclimber.xml")) {
        Log::debug("Couldn't load settings file, exiting...");
        return;
    }
    Settings::loadSaved("RailClimber");

    Tr::initialize();

    Log::debug("Before painter init");
    painter = Painter::getInstance();
    painter->initialize(screenW, screenH);
    Log::debug("After painter init");

    game = Game::getInstance();
    game->initialize(screenW, screenH);
    Log::debug("After game init");

    //__android_log_print(ANDROID_LOG_INFO, "RailClimber", "Init finished");
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_cleanup(JNIEnv *env, jobject obj) {
    if (painter) {
        painter->cleanup();
    }
    finished = true;
    if (game) {
        game->cleanup();
    }
    Settings::unloadSaved();
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_draw(JNIEnv *env, jobject obj) {
    if (!finished) {
        painter->setScroll(game);
        painter->drawFrame(game);
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_update(JNIEnv *env, jobject obj) {
    if (!finished) {
        game->updateGame(20);
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_touchPress(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    if (!finished) {
        game->handleTouchPress(x, y);
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_touchMove(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    //	Log("HTM - thread %d - %f %f", gettid(), x, y);
    if (!finished) {
        game->handleTouchMove(x, y);
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_touchRelease(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    if (!finished) {
        game->handleTouchRelease(x, y);
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_setFPS(JNIEnv *env, jobject obj, jfloat fps) {
    if (!finished) {
        if (game->getWorld()->secondsUpdated) {
            painter->updateTimeTextures(fps, game->getWorld()->levelTimer, game->showFPS);
            game->getWorld()->secondsUpdated = false;
        }
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_backButtonPressed(JNIEnv *env, jobject obj) {
    if (!finished) {
        game->handleBackAction();
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_createGraphics(JNIEnv *env, jobject obj) {
    if (!finished) {
        Log::debug("Before createGraphics, im = %d, om = %d", game->inMenu, game->overlayMenu);
        game->destroyGUI();
        painter->destroyGraphics();
        Log::debug("DSTROYD");
        painter->createGraphics();
        Log::debug("CRG");
        game->createGUI();
        Log::debug("After createGraphics, im = %d, om = %d", game->inMenu, game->overlayMenu);
    }
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_pause(JNIEnv *env, jobject obj) {
    game->pauseGame(true);
    Log::debug("Native - pause");
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_resume(JNIEnv *env, jobject obj) {
    game->pauseGame(false);
    Log::debug("Native - resume, im = %d, om = %d", game->inMenu, game->overlayMenu);
}

JNIEXPORT void JNICALL Java_net_holak_railclimber_RailClimberNative_requestDebugAction(JNIEnv *env, jobject obj) {
    const_cast<Menu*> (game->getMenu())->setActiveScreen("Debug");
}
