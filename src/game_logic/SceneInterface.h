#pragma once

#include "Vec2.h"
#include "3rdparty/nano-signal-slot/nano_signal_slot.hpp"

class SceneInterface {
public:
    enum SceneChangeType {
        Push,
        Replacement
    };

    virtual bool handleTouchPress(const Vec2& inGameXY) { return false; }
    virtual bool handleTouchMove(const Vec2& inGameXY) { return false; }
    virtual bool handleTouchRelease(const Vec2& inGameXY) { return false; }
    virtual bool handleBackAction() { return false; }
    virtual bool isTransparent() const { return false; }
    virtual void render() = 0;
    virtual const char* getSceneId() const = 0;

    virtual void loadAssets() { }
    virtual void reloadAssets() { }
    virtual void unloadAssets() { }

    virtual ~SceneInterface() {}

    Nano::Signal<bool(const char* sceneName, SceneChangeType changeType)> sceneChangeRequested;
    Nano::Signal<void()> finished;
};
