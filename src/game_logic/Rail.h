#pragma once

#include "RenderDriverInterface.h"
#include "PolygonalObject.h"
#include "Renderable.h"
#include <list>

class World;
class Enemy;

class Rail : public PolygonalObject {
public:
    Rail(World *w);
    virtual ~Rail();

    virtual void render(RenderDriverInterface *renderDriver) { renderDriver->drawRail(this); }

    void step();
    void disableRail();

    mutable float *boxCache[3]; //cache for the "box around line" coordinates for the rail
    mutable float *aaOverdrawCache[6];

    bool deadly;
    float deadlyProgress;
    float deadlyDir;
    int deadlyStage;
    int stageTimer;
    int stageTimes[3];

    bool disabled;
    int disabledTimer;

    //list<Enemy*> attachedEnemies;
    bool attachedPlayer;

    World *world;
};
