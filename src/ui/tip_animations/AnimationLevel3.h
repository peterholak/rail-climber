#pragma once

#include "ui/AnimationWidget.h"
#include "ui/IAnimationCallback.h"
#include "ui/Button.h"
#include "game_logic/WaveGenerator.h"

class AnimationLevel3 : public AnimationWidget, public IAnimationCallback {
public:
    AnimationLevel3(Painter *painter, MenuScreen *menu, const std::string &_name, float _x, float _y, float _w, float _h);
    virtual ~AnimationLevel3();

    void draw();
    void reload();
    void unload() { }
    void tick(bool debugManual = false);
    void screenInit();

    void animationCallback(int block);
    void runInGLThread(void *data = NULL);

private:
    Texture *texFingerDown, *texFingerUp;
    Texture *texFinger;
    Vec2 mFingerPos;
    int mWaveBlock, mFinishBlock;
    Movement mFingerMovement;
    WaveGenerator mWaveGen;
    Button *mButton;
};
