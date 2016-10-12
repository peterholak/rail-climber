#include "AnimationLevel4.h"
#include "ui/MenuScreen.h"

using namespace std;

AnimationLevel4::AnimationLevel4(Painter *painter, MenuScreen *menu, const string &_name, float _x, float _y, float _w, float _h)
: AnimationWidget(painter, _name, _x, _y, _w, _h) {
    mMenu = menu;
    mButton = NULL;
//    texFingerDown = &painter->texFingerDown;
//    texFingerUp = &painter->texFingerUp;
    texFinger = texFingerUp;
//    mFingerPos = Vec2(x + width * 0.2f, y - height * 0.8f);

    mFadePhase = 0;
    mClickBlock = 0;
    mAnimationStage = 0;
    mBallPhase = 0;
/*
    mRails[0] = new Rail(NULL);
    mRails[0]->create(4, x + width * 0.1, y - height * 0.5, x + width * 0.3, y - height * 0.45, x + width * 0.7, y - height * 0.56, x + width * 0.7, y - height * 0.41);
    mRails[1] = new Rail(NULL);
    mRails[1]->create(3, x + width * 0.2, y - height * 0.65, x + width * 0.4, y - height * 0.65, x + width * 0.8, y - height * 0.71);
    mRails[2] = new Rail(NULL);
    mRails[2]->create(4, x + width * 0.15, y - height * 0.8, x + width * 0.65, y - height * 0.73, x + width * 0.55, y - height * 0.89, x + width * 0.4, y - height * 0.89);
    */
}

AnimationLevel4::~AnimationLevel4() {
    for (int i = 0; i < 3; i++) {
        delete mRails[i];
    }
}

void AnimationLevel4::draw() {
//    GLfloat r10 = mPainter->getRatio() * 10.0f;

/*    GLfloat ptx = r10 - 2.5f, pty = 7.5f;
    GLfloat mixCoords[] = {
        0.0f, 0.0f, 0.0f, ptx, 0.0f, 0.0f, ptx, pty, 0.0f,
        0.0f, 0.0f, 0.0f, ptx, pty, 0.0f, 0.0f, pty, 0.0f,
        ptx, 0.0f, 0.0f, r10, 0.0f, 0.0f, r10, pty, 0.0f,
        ptx, 0.0f, 0.0f, r10, pty, 0.0f, ptx, pty, 0.0f,
        0.0f, pty, 0.0f, ptx, pty, 0.0f, ptx, 10.0f, 0.0f,
        0.0f, pty, 0.0f, ptx, 10.0f, 0.0f, 0.0f, 10.0f, 0.0f
    };

    GLfloat restCoords[] = RECTANGLE(ptx, pty, 2.5f, 2.5f);

    GLfloat c = 0.012 * mFadePhase;
    GLfloat n = c / 1.2;
    GLfloat clr[] = {
        0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, n, 0.0f, 0.0f, 0.0f, c,
        0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, n, 0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, n, 0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, c, 0.0f, 0.0f, 0.0f, n, 0.0f, 0.0f, 0.0f, c
    };
    GLfloat restClr[] = {
        0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, n,
        0.0f, 0.0f, 0.0f, 0.0f
    };

    mPainter->prgDefault.bind();
    mPainter->prgDefault.resetDefaultAttributes();
    mPainter->prgDefault.setUniform("noTexture", 1);
    mPainter->prgDefault.setPosition(mixCoords);
    mPainter->prgDefault.setColor(clr);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    mPainter->prgDefault.setPosition(restCoords);
    mPainter->prgDefault.setColor(restClr);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mPainter->prgDefault.setUniform("noTexture", 0);

    mPainter->drawTextureRect(mPainter->texBall, mBallPos.x(), mBallPos.y(), false, 0.8f);
    mPainter->drawTextureRect(mPainter->texGlare, mBallPos.x(), mBallPos.y(), false, 0.8f);
    glBindTexture(GL_TEXTURE_2D, mPainter->texRail[mPainter->activeBkg].id);
    for (int i = 0; i < 3; i++) {
        mPainter->drawRail(mRails[i]);
    }

    mPainter->drawTextureRect(*texFinger, mFingerPos.x(), mFingerPos.y(), false, 1);*/
}

void AnimationLevel4::screenInit() {
    mFadePhase = 0;
    mBallPhase = 0;
    mAnimationStage = 0;

    mGravityBoost = false;

//    mButton = static_cast<Button*> (mMenu->getActiveScreen()->findWidget("TipsHide"));
    mButton->setEnabled(false);
/*    mButton->setText("...");

    GLfloat r10 = mPainter->getRatio() * 10.0f;

    mBallPos.set(x + width * 0.5, y + height * 0.4);

    mFingerMovement.clear();
    mFingerMovement.registerAnimationCallback(this);
    mFingerMovement.add(mFingerPos, 80);
    mFingerMovement.add(Vec2(r10 - 0.5f, 9.5f), 50);
    mClickBlock = mFingerMovement.add(Vec2(r10 - 0.5f, 9.5f), 30);
    mFingerMovement.add(Vec2(r10 - 0.5, 9.5f), 5);*/
}

void AnimationLevel4::tick(bool debugManual) {
    if (mAnimationStage == 0) {
        if (mFadePhase < 80) {
            mFadePhase += 2;
        } else {
            mAnimationStage = 1;
        }
    } else if (mAnimationStage == 1) {
        if (mBallPhase < 130) {
            mBallPhase++;
        } else {
            mAnimationStage = 2;
        }
    } else if (mAnimationStage == 2) {
        mGravityBoost = false;
//        mButton->setText("Ok");
        mButton->setEnabled(true);
        if (mFadePhase > 0) {
            mFadePhase -= 2;
        }
    }

    mFingerMovement.tick(debugManual);
    mFingerPos = mFingerMovement.getPosition();

    if (!mGravityBoost) {
        mBallVel += Vec2(0, -0.00005f);
    } else {
        mBallVel += Vec2(0, -0.1f);
    }
    mBallPos += mBallVel;
}

void AnimationLevel4::animationCallback(int block) {
    if (block == mClickBlock) {
        mGravityBoost = true;
        texFinger = texFingerDown;
    }
}

void AnimationLevel4::reload() {

}

void AnimationLevel4::runInGLThread(void *data) {

}
