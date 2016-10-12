#pragma once

#include "ui/Widget.h"
#include <cstdio>
#include <vector>
#include <list>
#include "game_logic/Vec2.h"
#include "ui/IAnimationCallback.h"

class MenuScreen;

class AnimationWidget : public Widget {
public:
    AnimationWidget(Painter *painter, const std::string &_name, float _x, float _y, float _w, float _h);
    virtual ~AnimationWidget();

    static AnimationWidget* getAnimationForLevel(Painter *painter, MenuScreen *menu, int level, float _x, float _y, float _w, float _h);

    class Movement {
    public:
        Movement();

        int add(const Vec2 &v, unsigned int t);
        int addArcMovement(const Vec2 &center, float radius, float startAngle, float endAngle, int precision, unsigned int t, unsigned int tMoveInto = 0);
        int addArcMovement(const Vec2 &point1, const Vec2 &point2, float radius, int precision, unsigned int t, unsigned int tMoveInto = 0, bool leftHand = true, bool inside = true);
        void clear();
        void tick(bool debugManual = false);
        void reset();

        bool finished() const {
            return mFinished;
        }
        static int getPointQuadrant(const Vec2 &point, const Vec2 &center);

        const Vec2& getPosition() const {
            return mPosition;
        }

        void registerAnimationCallback(IAnimationCallback *handler);
        void unregisterAnimationCallback(IAnimationCallback *handler);

    private:
        std::vector<std::pair<Vec2, unsigned int> > mTargets;
        std::vector<unsigned int> mBlockDividers;
        std::list<IAnimationCallback*> mCallbacks;
        bool mFinished;
        unsigned int mBlock;
        unsigned int mCurrent; //current part
        unsigned int mPhase, mCurrentDuration; //the tick inside the current part and the number of ticks that this part has
        Vec2 mVelocity, mPosition;
    };

protected:
    MenuScreen *mMenu;
};
