#include "AnimationWidget.h"
#include "ui/tip_animations/AnimationLevel1.h"
#include "ui/tip_animations/AnimationLevel3.h"
#include "ui/tip_animations/AnimationLevel4.h"
#include "ui/tip_animations/AnimationLevel7.h"
#include <cstdio>
#include <cmath>

using namespace std;

AnimationWidget::AnimationWidget(Painter *painter, const string &_name, float _x, float _y, float _w, float _h)
: Widget(), mMenu(nullptr) {

}

AnimationWidget::~AnimationWidget() {

}

AnimationWidget* AnimationWidget::getAnimationForLevel(Painter *painter, MenuScreen *menu, int level, float _x, float _y, float _w, float _h) {
    char name[16];
    snprintf(name, 16, "AnimatedTip%d", level);
    switch (level) {
/*        case 1: return new AnimationLevel1(painter, menu, name, _x, _y, _w, _h);
        case 4: return new AnimationLevel3(painter, menu, name, _x, _y, _w, _h);
        case 7: return new AnimationLevel4(painter, menu, name, _x, _y, _w, _h);
        case 9: return new AnimationLevel7(painter, menu, name, _x, _y, _w, _h);*/
    }
    return NULL;
}

AnimationWidget::Movement::Movement() {
    reset();
}

/**
 * Adds a target point
 *
 * @param v target position vector
 * @param t duration of the movement from previous vector to this one
 * (should be 0 for first point unless you want the object to pause there)
 */
int AnimationWidget::Movement::add(const Vec2 &v, unsigned int t) {
    mTargets.push_back(make_pair(v, t));
    mBlockDividers.push_back(mTargets.size());
    return mBlockDividers.size();
}

void AnimationWidget::Movement::clear() {
    mTargets.clear();
    mBlockDividers.clear();
    mCallbacks.clear();
    reset();
}

/**
 *  2 | 1
 * ---+---
 *  3 | 4
 *
 * -1 on error
 *
 * @param point
 * @param center
 * @return
 */
int AnimationWidget::Movement::getPointQuadrant(const Vec2 &point, const Vec2 &center) {
    if (point.x() < center.x() && point.y() > center.y()) {
        return 2;
    } else if (point.x() > center.x() && point.y() > center.y()) {
        return 1;
    } else if (point.x() > center.x() && point.y() < center.y()) {
        return 4;
    } else if (point.x() < center.x() && point.y() < center.y()) {
        return 3;
    }
    return -1;
}

/**
 *
 * @param center
 * @param radius
 * @param startAngle
 * @param endAngle
 * @param precision number of segments that form the arc, minimum is 1 (straight line)
 * @param t
 * @param tMoveInto
 */
int AnimationWidget::Movement::addArcMovement(const Vec2 &center, float radius, float startAngle, float endAngle, int precision, unsigned int t, unsigned int tMoveInto) {
    if (mTargets.size() > 0 && tMoveInto == 0) {
        tMoveInto = 1;
    }

    float angleStep = (endAngle - startAngle) / precision;
    int timeStep = t / precision;

    for (int i = 0; i <= precision; i++) {
        float angle = startAngle + (i * angleStep);
        Vec2 point = center + Vec2(cos(angle) * radius, sin(angle) * radius);
        mTargets.push_back(make_pair(point, (i == 0 ? tMoveInto : timeStep)));
    }
    mBlockDividers.push_back(mTargets.size());
    return mBlockDividers.size();
}

/**
 * An arc where user specifies two points that the arc passes through and the desired radius.
 * The center is then calculated automatically.
 *
 * @param point1
 * @param point2
 * @param radius
 * @param precision
 * @param t
 * @param tMoveInto
 */
int AnimationWidget::Movement::addArcMovement(const Vec2 &point1, const Vec2 &point2, float radius, int precision, unsigned int t, unsigned int tMoveInto, bool leftHand, bool inside) {
    if (mTargets.size() > 0 && tMoveInto == 0) {
        tMoveInto = 1;
    }

    //why the abs?
    Vec2 middle(/*abs*/(point1.x() + point2.x()) / 2, /*abs*/(point1.y() + point2.y()) / 2);
    float half = (point1 - point2).length() / 2;

    if (radius < half) {
        radius = half + 0.01f;
    }

    float distToCenter = sqrt(radius * radius - half * half);

    if (leftHand) {
        distToCenter = -distToCenter;
    }

    Vec2 dirPar = point2 - point1;
    Vec2 center = middle + dirPar.perpendicular(true).toDistance(distToCenter);

    float pi = 3.141592654f;

    int p1q = getPointQuadrant(point1, center);
    int p2q = getPointQuadrant(point2, center);

    //these are probably wrong in about half the cases
    float startAngle = acos((point1.x() - center.x()) / radius);
    float endAngle = acos((point2.x() - center.x()) / radius);

    //3rd and 4th quadrants
    if (p1q > 2) {
        startAngle = 2 * pi - startAngle;
    }

    if (p2q > 2) {
        endAngle = 2 * pi - endAngle;
    }

    if (inside) {
        if (abs((double) (endAngle - startAngle)) > pi) {
            if (endAngle > startAngle) {
                endAngle -= 2 * pi;
            } else {
                startAngle -= 2 * pi;
            }
        }
    } else {
        if (abs((double) (endAngle - startAngle)) < pi) {
            if (endAngle > startAngle) {
                endAngle -= 2 * pi;
            } else {
                startAngle -= 2 * pi;
            }
        }
    }

    addArcMovement(center, radius, startAngle, endAngle, precision, t, tMoveInto);

    return mBlockDividers.size();
}

/**
 * movement:
 *   1. [current=0] warp to position 0, stay there for mTargets[0].second ticks
 *   2. [current=1] move from <current-1> to <current> in <current> ticks
 *   3. [current=1] repeat movement by a pre-calculated difference, warp on the last tick in this phase
 *   3. current++, if there are any more, calculate the step difference, repeat step 2, otherwise stop
 */
void AnimationWidget::Movement::tick(bool debugManual) {
    if (mFinished || mTargets.size() < 1) {
        return;
    }

    if (mPhase == 0 && mCurrent == 0) {
        mPosition = mTargets[0].first;
        mCurrentDuration = mTargets[0].second;
        mPhase++;
        mVelocity = Vec2();
        if (mCurrentDuration == 0) {
            tick();
        }
    } else {
        mPosition += mVelocity;
        mPhase++;
        if (mPhase >= mCurrentDuration) {
            mPhase = 0;
            mPosition = mTargets[mCurrent].first;
            if (mTargets.size() > mCurrent + 1) {
                mCurrent++;
                if (mCurrent == mBlockDividers[mBlock]) {
                    mBlock++;
                    for (list<IAnimationCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it) {
                        (*it)->animationCallback(mBlock);
                    }
                }
                mCurrentDuration = mTargets[mCurrent].second;
                Vec2 diff = mTargets[mCurrent].first - mTargets[mCurrent - 1].first;
                mVelocity = diff.toDistance(diff.length() / mCurrentDuration);
            } else {
                mFinished = true;
            }
        }
    }
}

void AnimationWidget::Movement::reset() {
    mFinished = false;
    mCurrent = 0;
    mPhase = 0;
    mCurrentDuration = 0;
    mBlock = 0;
}

void AnimationWidget::Movement::registerAnimationCallback(IAnimationCallback *handler) {
    mCallbacks.push_back(handler);
}

void AnimationWidget::Movement::unregisterAnimationCallback(IAnimationCallback *handler) {
    mCallbacks.remove(handler);
}
