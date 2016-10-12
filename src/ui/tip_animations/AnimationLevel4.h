#pragma once

#include "ui/AnimationWidget.h"
#include "ui/IAnimationCallback.h"
#include "ui/Button.h"
#include "game_logic/Rail.h"

class AnimationLevel4 : public AnimationWidget, public IAnimationCallback {
public:
    AnimationLevel4(Painter *painter, MenuScreen *menu, const std::string &_name, float _x, float _y, float _w, float _h);
    virtual ~AnimationLevel4();

    void draw();
    void reload();
    void unload() { }
    void tick(bool debugManual = false);
    void screenInit();

    void animationCallback(int block);
    void runInGLThread(void *data = NULL);

private:
    Texture *texFinger, *texFingerUp, *texFingerDown;
    Vec2 mFingerPos, mBallPos, mBallVel;
    Movement mFingerMovement;
    Button *mButton;
    Rail *mRails[3];
    int mFadePhase, mBallPhase, mAnimationStage;
    int mClickBlock;
    bool mGravityBoost;
};
