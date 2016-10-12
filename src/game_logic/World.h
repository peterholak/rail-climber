#pragma once

#include "game_logic/Vec2.h"
#include "game_logic/Rail.h"
#include "game_logic/StaticObject.h"
#include <vector>
#include <list>
#include <set>
#include <string>
#include <ctime>
#include <Box2D/Box2D.h>
#include "3rdparty/nano-signal-slot/nano_signal_slot.hpp"

class WaveGenerator;
class Enemy;
class Bullet;
class Vibrator;

class World : public b2QueryCallback, public b2ContactListener {
public:

    enum PlayerState {
        FreeState,
        AttachingState,
        AttachedState,
        DeadState
    };

    struct MyUserData {

        enum Type {
            PlayerType,
            RailType,
            FloorType,
            WallType,
            StaticType,
            EnemyType
        } type;

        Rail *rail;
        StaticObject *staticObject;
        Enemy *enemy;

        MyUserData(Type _type) {
            type = _type;
            rail = nullptr;
            staticObject = nullptr;
            enemy = nullptr;
        }

        MyUserData(Rail *_rail) {
            type = RailType;
            rail = _rail;
            staticObject = nullptr;
            enemy = nullptr;
        }

        MyUserData(StaticObject *so) {
            type = StaticType;
            staticObject = so;
            rail = nullptr;
            enemy = nullptr;
        }

        MyUserData(Enemy *e) {
            type = EnemyType;
            enemy = e;
            rail = nullptr;
            staticObject = nullptr;
        }
    };

    struct Player {
        Vec2 pos;

        b2CircleShape *shape;
        b2Body *body;
        b2Fixture *fixture;

        PlayerState state;
        Rail *rail;
        unsigned int railSegment;
        int direction;
        int attachingStep;
        Vec2 attachingPoint;

        int detachCounter;
        int deadCounter;

        std::set<Enemy*> touchingEnemies;

        bool touchedGround;
        std::set<StaticObject*> touching;
        std::set<StaticObject*> forceFieldTouching;
        bool touchingFloor;

        bool inForceField;

        void setFilter(int mask);
        void reset(bool clearBody = false);
        bool canFire() const;
        void enterForceField();
        void leaveForceField();
    };

    struct Grass {
        Vec2 start, end;
        bool reversed;
        float texCache[3][8];
        float pointCache[3][12];
    };

    World();
    virtual ~World();

    Player& getPlayer() {
        return mPlayer;
    }

    const Player& getPlayer() const {
        return mPlayer;
    }

    const std::list<Rail*>& getRails() const {
        return mRails;
    }

    const std::list<StaticObject*>& getStaticObjects() const {
        return mStatics;
    }

    const std::list<StaticObject*>& getHazards() const {
        return mHazards;
    }

    const std::list<StaticObject*>& getForceFields() const {
        return mForceFields;
    }

    const std::list<WaveGenerator*>& getWaveGenerators() const {
        return mWaveGens;
    }

    const std::list<Grass*>& getGrass() const {
        return mGrass;
    }

    const std::list<Enemy*>& getEnemies() const {
        return mEnemies;
    }

    const std::list<Bullet*>& getBullets() const {
        return mBullets;
    }

    void step(float period);
    void pause(bool pause);
    void firePlayer(float x, float y);
    void setGravityBoost(bool boost);
    bool preProcessTouch(float igx, float igy);
    void touchPress(float igx, float igy);
    void fireBullet(const Vec2 &origin, const Vec2& direction);
    void killPlayer(bool stop = true);
    void killEnemy(Enemy *e, bool dispose = false);

    const b2World *getBoxWorld() const {
        return mBoxWorld;
    }

    void loadLevel(const std::string &data, int version = 1);
    void loadLevelFromFile(const std::string &filename, int version = 1);
    void unloadLevel();
    void showOverlayMenu(const std::string &screen);

    void destroyBufferObjects();

    bool gravityBoost, gbFirstHit;
    bool paused;
    // TODO: these should definitely be floats
    int levelWidth, levelH;
    int levelTimer, lastTimer;
    bool justStarted;
    bool secondsUpdated;

    void addRail(Rail *r);
    void addStaticObject(StaticObject *so);
    void addWaveGenerator(WaveGenerator *wg);
    void addEnemy(Enemy *e);

    void addGrass(Grass *g) {
        calculateGrassData(g);
        mGrass.push_back(g);
    }

    void createWorld();

private:
    const Vibrator &mVibrator;
    long long mLastStep;
    int mLastStepLog;
    std::list<int> mLSTimes;
    long long mLastTick;
    Player mPlayer;
    Enemy *mQueriedEnemy;
    int mGravityStep;
    std::list<Rail*> mRails;
    std::list<StaticObject*> mStatics;
    std::list<StaticObject*> mForceFields;
    std::list<WaveGenerator*> mWaveGens;
    std::list<Grass*> mGrass;
    std::list<Enemy*> mEnemies;
    std::list<Bullet*> mBullets;
    std::list<StaticObject*> mHazards;
    b2World *mBoxWorld;
    b2BodyDef *mGroundBodyDef;
    b2Body *mGroundBody;
    b2PolygonShape *mGroundShape;

    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) { }
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) { }
    bool ReportFixture(b2Fixture *fixture);

    void processBullets();
    void queryRailAttach();
    void attachToRail(Rail *r, int segmentIndex, const b2Vec2 &nearestPoint);
    bool detachFromRail();
    void moveOnRail();
    void moveEnemies();
    bool canDetach();
    bool insideStaticObject(StaticObject *so);

    void calculateGrassData(Grass *grass);

public:
    Nano::Signal<void()> playerDied;
    Nano::Signal<void()> levelFinished;

    void render(RenderDriverInterface *renderDriver);

public:
    static float playerRadius;
    static float playerTouchRadius;
    static float maximumForce;
    static float attachDistance;
    static float attachSpeed;
    static float detachTime;
    static float wallThickness;
    static float defaultGravity;
    static float enemyRestitution;
    static int defaultFilter;
    static bool canFly;
    static bool invincible;
    static int gravityBoostDuration;
};
