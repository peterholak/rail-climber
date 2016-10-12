#pragma once

#include <Box2D/Box2D.h>
#include "game_logic/Vec2.h"
#include "game_logic/Rail.h"

class World;

class Bullet : public Renderable {
public:
    Vec2 pos;
    Vec2 velocity;

    virtual void render(RenderDriverInterface *renderDriver) { renderDriver->drawBullet(this); }
};

class Enemy : public Renderable {
public:
    static float halfWidth;
    static float halfHeight;
    static float firingRange;
    static int firingDelay;
    static int mouthDelay;
    static float bulletSpeed;
    static float bulletSize;

    enum EnemyState {
        Attached, Detached, Dying, Attaching
    };
    Enemy(World *w);
    ~Enemy();

    virtual void render(RenderDriverInterface *renderDriver) { renderDriver->drawEnemy(this); }

    void step(const Vec2 &playerPos);
    void attachToRail(Rail *r, int segmentIndex, const b2Vec2 &nearestPoint);
    void setEyePosition(const Vec2 &playerPos);
    void die();
    void detach(int filterMask = 0xFFFE);

    b2Body *body;
    b2Fixture *fixture;

    int eyePosition;
    bool mouthOpen;
    int firingCounter;
    int closeMouthCounter;

    Vec2 pos;
    bool active;
    EnemyState state;
    Rail *rail;
    unsigned int railSegment;
    int attachingStep;
    float targetAngle;
    int angleCounter;
    Vec2 attachingPoint;
    int direction;
    Rail *disabledRail;
    int disabledTimer;

private:
    World *mWorld;
    void moveOnRail();
    float normalizeAngle(float angle);

public:
    bool fast;
};
