#include "game_logic/World.h"
#include "AnimationLevel1.h"
#include "game_logic/Vec2.h"
#include "ui/MenuScreen.h"
#include "ui/Button.h"

using namespace std;

AnimationLevel1::AnimationLevel1(Painter *painter, MenuScreen *menu, const string &_name, float _x, float _y, float _w, float _h)
: AnimationWidget(painter, _name, _x, _y, _w, _h) {
/*    texBall = &mPainter->texBall;
    texGlare = &mPainter->texGlare;
    texFingerUp = &mPainter->texFingerUp;
    texFingerDown = &mPainter->texFingerDown;*/
    texFinger = texFingerUp;
    mMenu = menu;
    mButton = NULL;
    mFinished = false;
    mDrawPointer = false;
    mDragBlock = 0;
}

AnimationLevel1::~AnimationLevel1() {

}

void AnimationLevel1::draw() {
    //mPainter->drawTextureRect(*texBall, mBallPos.x() - 0.4, mBallPos.y() + 0.4, true, 0.8);
    //mPainter->drawTextureRect(*texGlare, mBallPos.x() - 0.4, mBallPos.y() + 0.4, true, 0.8);

    if (mDrawPointer) {
        Vec2 player = mBallPos, force = mFingerPos - mBallPos;
        Vec2 r1, r2, r3, r4;

        float len = force.length();

        Vec2 start = player + force.toDistance(World::playerRadius + 0.1f);
        Vec2 mid = player + force.toDistance(World::playerRadius + 0.25f + len * 0.06f);
        Vec2 end = player + force;

        Vec2::boxAroundLine(0.35f, start, end, r1, r2, r3, r4);
        GLfloat coords[] = {
            r1.x(), r1.y(), 0,
            end.x(), end.y(), 0,
            mid.x(), mid.y(), 0,

            r3.x(), r3.y(), 0,
            end.x(), end.y(), 0,
            mid.x(), mid.y(), 0
        };

        float e = 0.4f;
        float a = 0.3f;
        GLfloat color[] = {e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a, e, e, e, a};

/*        mPainter->prgDefault.bind();
        mPainter->prgDefault.resetDefaultAttributes();
        mPainter->prgDefault.setUniform("noTexture", 1);
        mPainter->prgDefault.setPosition(coords);
        mPainter->prgDefault.setColor(color);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        mPainter->prgDefault.setUniform("noTexture", 0);*/
    }

    //mPainter->drawTextureRect(*texFinger, mFingerPos.x(), mFingerPos.y(), true, 1);
    //painter->drawTextureRect(*texBall, mCenterVec.x(), mCenterVec.y() + 0.8, true, 0.8);
}

void AnimationLevel1::screenInit() {
    mDrawPointer = false;
    mFinished = false;
    texFinger = texFingerUp;
//    mButton = static_cast<Button*> (mMenu->getActiveScreen()->findWidget("TipsHide"));
    mButton->setEnabled(false);
    mButton->setText("...");
/*
    float block = width() / 10;
    mBallPos.set(x + 2 * block, y - height);
    mFingerPos.set(x + width - 3 * block, y - block);
    mBallVel.set(0, 0);

    mFingerMovement.clear();
    mFingerMovement.registerAnimationCallback(this);

    mFingerMovement.add(mFingerPos, 20);
    mFingerMovement.addArcMovement(mFingerPos, mBallPos, 3, 12, 40, 0, true, true);
    mDragBlock = mFingerMovement.add(mBallPos, 30);
    mFingerMovement.addArcMovement(mBallPos, mFingerPos, 4, 12, 80, 0, false, true);
    mFingerMovement.add(mFingerPos, 40);*/
}

void AnimationLevel1::tick(bool debugManual) {
    //if (!debugManual)
    //return;
    if (!mFinished) {
        mFingerMovement.tick();
        mFingerPos = mFingerMovement.getPosition();
        if (mFingerMovement.finished()) {
            mDrawPointer = false;
            texFinger = texFingerUp;
            mButton->setText("Ok");
            mButton->setEnabled(true);
            mFinished = true;
            mBallVel = (mFingerPos - mBallPos).toDistance(0.2);
        }
    } else {
        mBallPos += mBallVel;
        mBallVel += Vec2(0, -0.006);
    }
}

void AnimationLevel1::reload() {

}

void AnimationLevel1::runInGLThread(void *data) {

}

void AnimationLevel1::animationCallback(int block) {
    if (block == mDragBlock) {
        mDrawPointer = true;
        texFinger = texFingerDown;
    }
}
