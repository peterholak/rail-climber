#include "AnimationLevel3.h"
#include "ui/MenuScreen.h"

using namespace std;

AnimationLevel3::AnimationLevel3(Painter *painter, MenuScreen *menu, const string &_name, float _x, float _y, float _w, float _h)
: AnimationWidget(painter, _name, _x, _y, _w, _h) {
    mMenu = menu;
    mButton = NULL;
/*    texFingerUp = &mPainter->texFingerUp;
    texFingerDown = &mPainter->texFingerDown;*/
    texFinger = texFingerUp;
    mWaveBlock = 0;
    mFinishBlock = 0;
}

AnimationLevel3::~AnimationLevel3() {

}

void AnimationLevel3::draw() {
//    mPainter->drawWaveGenerator(&mWaveGen);
//    mPainter->drawTextureRect(*texFinger, mFingerPos.x(), mFingerPos.y(), true, 1);
}

void AnimationLevel3::screenInit() {
//    mButton = static_cast<Button*> (mMenu->getActiveScreen()->findWidget("TipsHide"));
/*    mButton->setEnabled(false);
    mButton->setText("...");

    mFingerPos = Vec2(x, y - height);

    mWaveGen.x = x + width / 2;
    mWaveGen.y = y - height / 2 - WaveGenerator::radius * 2;

    mFingerMovement.clear();
    mFingerMovement.registerAnimationCallback(this);
    mFingerMovement.add(mFingerPos, 20);
    mFingerMovement.addArcMovement(mFingerPos, Vec2(mWaveGen.x, mWaveGen.y), 3, 8, 40, 0, false, true);
    mWaveBlock = mFingerMovement.add(Vec2(mWaveGen.x, mWaveGen.y), 30); //FIXME: callback doesn't work with last block
    mFinishBlock = mFingerMovement.add(Vec2(mWaveGen.x, mWaveGen.y), 20);
    mFingerMovement.add(Vec2(mWaveGen.x, mWaveGen.y), 10);*/
}

void AnimationLevel3::tick(bool debugManual) {
    if (!mFingerMovement.finished()) {
        mFingerMovement.tick();
        mFingerPos = mFingerMovement.getPosition();
    }

    if (mWaveGen.active) {
        mWaveGen.step();
    }
}

void AnimationLevel3::animationCallback(int block) {
    if (block == mWaveBlock) {
        texFinger = texFingerDown;
        mWaveGen.activate();
    } else if (block == mFinishBlock) {
        texFinger = texFingerUp;
        mButton->setText("Ok");
        mButton->setEnabled(true);
    }
}

void AnimationLevel3::reload() {
    //TODO!!!
}

void AnimationLevel3::runInGLThread(void *data) {

}
