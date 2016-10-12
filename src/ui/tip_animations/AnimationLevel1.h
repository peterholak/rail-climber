#pragma once

#include "ui/AnimationWidget.h"
#include "ui/IAnimationCallback.h"
#include "graphics/Texture.h"

class Button;

class AnimationLevel1 : public AnimationWidget, public IAnimationCallback {
public:
    AnimationLevel1(Painter *painter, MenuScreen *menu, const std::string &_name, float _x, float _y, float _w, float _h);
    virtual ~AnimationLevel1();

    void draw();
    void reload();
    void unload() { }
    void tick(bool debugManual = false);
    void screenInit();

    void animationCallback(int block);
    void runInGLThread(void *data = NULL);

private:
    Texture *texBall, *texGlare, *texFinger;
    Texture *texFingerUp;
    Texture *texFingerDown;
    int mDragBlock;
    bool mDrawPointer, mFinished;
    Button *mButton;

    Vec2 mBallPos, mBallVel, mFingerPos, mCenterVec;

    Movement mFingerMovement;
};
