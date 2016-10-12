#include "World.h"

#include <sstream>
#include "platform/core.h"
#include "Sound.h"
#include "game_logic/WaveGenerator.h"
#include "game_logic/Enemy.h"
#include "Settings.h"
#include "GameScene.h"
#include "Logging.h"
#include "level_loader/LevelLoaderException.h"
#include "game_logic/CollisionFilter.h"

using namespace std;

float World::playerRadius;
float World::playerTouchRadius;
float World::maximumForce;
float World::attachDistance;
float World::attachSpeed;
float World::detachTime;
float World::wallThickness;
float World::defaultGravity;
float World::enemyRestitution;
bool World::canFly;
bool World::invincible;
int World::defaultFilter;
int World::gravityBoostDuration;

// TODO: vibration
World::World() : gravityBoost(false), paused(false),
levelWidth(0), levelH(0), levelTimer(0), lastTimer(0), secondsUpdated(true),
mVibrator(Vibrator()), mGravityStep(0) {

    mGroundBody = nullptr;
    mGroundBodyDef = nullptr;
    mGroundShape = nullptr;
    justStarted = true;
    gbFirstHit = false;
    mQueriedEnemy = nullptr;

    mLastTick = 0;
    mLastStep = 0;
    mLastStepLog = 0;
    playerRadius = Settings::getFloat("Player/Radius");
    playerTouchRadius = Settings::getFloat("Player/TouchRadius");
    maximumForce = Settings::getFloat("Player/MaxForce");
    attachDistance = Settings::getFloat("Rail/AttachDistance");
    attachSpeed = Settings::getFloat("Rail/AttachSpeed");
    detachTime = Settings::getFloat("Rail/DetachTime");
    wallThickness = Settings::getFloat("World/WallThickness");
    defaultGravity = Settings::getFloat("World/Gravity");
    gravityBoostDuration = Settings::getInt("GravityBoost/Duration");
    canFly = Settings::getBool("World/CanFly");
    invincible = Settings::getBool("World/Invincible");

    defaultFilter = CollisionFilter(CollisionFilter::NoCollision, CollisionFilter::Everything)
        .doNotCollideWith(CollisionFilter::Rail)
        .doNotCollideWith(CollisionFilter::Enemy)
        .getMask();

    WaveGenerator::radius = Settings::getFloat("WaveGenerator/Radius");
    WaveGenerator::touchRadius = Settings::getFloat("WaveGenerator/TouchRadius");
    WaveGenerator::maxRadius = Settings::getFloat("WaveGenerator/MaxRadius");
    WaveGenerator::duration = Settings::getFloat("WaveGenerator/Duration");
    WaveGenerator::power = Settings::getFloat("WaveGenerator/Power");

    enemyRestitution = Settings::getFloat("Enemy/Restitution");
    Enemy::firingRange = Settings::getFloat("Enemy/Range");
    Enemy::firingDelay = Settings::getInt("Enemy/FiringDelay");
    Enemy::bulletSize = Settings::getFloat("Enemy/BulletSize");
    Enemy::bulletSpeed = Settings::getFloat("Enemy/BulletSpeed");

    mBoxWorld = nullptr;
    mPlayer.pos.set(0, 0);
}

World::~World() {
    unloadLevel();
}

void World::createWorld() {
    mPlayer.reset(true);
    //initliaze the world
    mBoxWorld = new b2World(b2Vec2(0.0f, defaultGravity));
    mBoxWorld->SetContactListener(this);

    //player's physical representation
    b2BodyDef playerDef;
    playerDef.type = b2_dynamicBody;
    playerDef.position.Set(mPlayer.pos.x(), mPlayer.pos.y());
    playerDef.fixedRotation = false;
    mPlayer.body = mBoxWorld->CreateBody(&playerDef);
    mPlayer.shape = new b2CircleShape();
    mPlayer.shape->m_radius = playerRadius;
    b2FixtureDef playerFixtureDef;
    playerFixtureDef.shape = mPlayer.shape;
    playerFixtureDef.density = Settings::getFloat("Player/Density");
    playerFixtureDef.friction = Settings::getFloat("Player/Friction");
    playerFixtureDef.restitution = Settings::getFloat("Player/Restitution");
    playerFixtureDef.filter = CollisionFilter(CollisionFilter::Player, CollisionFilter::Everything)
        .doNotCollideWith(CollisionFilter::Rail)
        .doNotCollideWith(CollisionFilter::Enemy)
        .get();
    mPlayer.fixture = mPlayer.body->CreateFixture(&playerFixtureDef);
    mPlayer.fixture->SetUserData(new MyUserData(MyUserData::PlayerType));

    b2Filter wallFilter = CollisionFilter(CollisionFilter::Wall, CollisionFilter::Everything).get();

    //the ground
    mGroundBodyDef = new b2BodyDef();
    mGroundBodyDef->position.Set(0.0f, 0.0f);
    mGroundBody = mBoxWorld->CreateBody(mGroundBodyDef);
    mGroundShape = new b2PolygonShape();
    mGroundShape->SetAsBox(levelWidth, wallThickness);
    b2Fixture *groundFix = mGroundBody->CreateFixture(mGroundShape, 0.0f);
    groundFix->SetUserData(new MyUserData(MyUserData::FloorType));
    groundFix->SetFilterData(wallFilter);

    //left wall
    b2BodyDef *wallDef = new b2BodyDef();
    wallDef->position.Set(0.0f, 0.0f);
    b2Body *wallBody = mBoxWorld->CreateBody(wallDef);
    b2PolygonShape *wallShape = new b2PolygonShape();
    wallShape->SetAsBox(wallThickness, levelH);
    b2Fixture *wallFix = wallBody->CreateFixture(wallShape, 0.0f);
    wallFix->SetUserData(new MyUserData(MyUserData::WallType));
    wallFix->SetFilterData(wallFilter);

    //right wall
    wallDef = new b2BodyDef();
    wallDef->position.Set(levelWidth - wallThickness, 0.0f);
    wallBody = mBoxWorld->CreateBody(wallDef);
    wallShape = new b2PolygonShape();
    wallShape->SetAsBox(wallThickness, levelH);
    wallFix = wallBody->CreateFixture(wallShape, 0.0f);
    wallFix->SetUserData(new MyUserData(MyUserData::WallType));
    wallFix->SetFilterData(wallFilter);
}

void World::addStaticObject(StaticObject *so) {
    if (so->hazard) {
        mHazards.push_back(so);
    }else if (so->forceField){
        mForceFields.push_back(so);
    }else{
        mStatics.push_back(so);
    }

    const vector<Vec2> &points = so->getPoints();
    b2Vec2 *pts = new b2Vec2[points.size()];
    for (unsigned int i = 0; i < points.size(); i++) {
        const Vec2 &v = points[i];
        pts[i].Set(v.x(), v.y());
    }
    b2ChainShape shape;
    shape.CreateLoop(pts, points.size());
    b2BodyDef bodyDef;

    so->body = mBoxWorld->CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    if (so->hazard || so->forceField) {
        fixtureDef.filter.categoryBits = CollisionFilter::NoCollision; //don't collide with anything
    }else{
        fixtureDef.filter.categoryBits = CollisionFilter::Wall;
    }
    fixtureDef.filter.maskBits = CollisionFilter::Everything;
    so->fixture = so->body->CreateFixture(&fixtureDef);
    so->fixture->SetUserData(new MyUserData(so));
}

/**
 * Adds a rail
 */
void World::addRail(Rail *r) {
    //add the rail to the world
    mRails.push_back(r);

    //create a physical representation for the rail
    const vector<Vec2> &points = r->getPoints();
    b2Vec2 *pts = new b2Vec2[points.size()];
    for (unsigned int i = 0; i < points.size(); i++) {
        const Vec2 &v = points[i];
        pts[i].Set(v.x(), v.y());
    }
    b2ChainShape shape;
    shape.CreateChain(pts, points.size());
    b2BodyDef bodyDef;
    r->body = mBoxWorld->CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.filter = CollisionFilter(CollisionFilter::Rail, CollisionFilter::Everything)
        .doNotCollideWith(CollisionFilter::Wall)
        .get();
    r->fixture = r->body->CreateFixture(&fixtureDef);
    r->fixture->SetUserData(new MyUserData(r));
}

void World::addWaveGenerator(WaveGenerator *wg) {
    mWaveGens.push_back(wg);
}

void World::addEnemy(Enemy *e) {
    mEnemies.push_back(e);

    e->pos.set(e->pos.x() + Enemy::halfWidth, e->pos.y() - Enemy::halfHeight);
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(e->pos.x(), e->pos.y());
    bodyDef.type = b2_dynamicBody;
    e->body = mBoxWorld->CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    b2PolygonShape shape;
    shape.SetAsBox(Enemy::halfWidth - 0.05, Enemy::halfHeight - 0.05);
    fixtureDef.shape = &shape;
    fixtureDef.density = 100.0f;
    fixtureDef.filter = CollisionFilter(CollisionFilter::Enemy, CollisionFilter::Everything).get();
    fixtureDef.restitution = enemyRestitution;
    e->fixture = e->body->CreateFixture(&fixtureDef);
    e->fixture->SetUserData(new MyUserData(e));
}

void World::step(float period) {
    if (paused) {
        return;
    }

    justStarted = false;

    long long cl = RailClimberPlatform::getTicks();
    if (mLastTick == 0) {
        mLastTick = cl;
    }

    levelTimer += cl - mLastTick;
    if (levelTimer - lastTimer >= 1000) {
        secondsUpdated = true;
        lastTimer = levelTimer;
    }
    mLastTick = cl;
    //do the physics calculations
    mBoxWorld->Step(period, 6, 2);

    processBullets();

    //move game objects to match their physical representations
    b2Vec2 position = mPlayer.body->GetPosition();
    mPlayer.pos.set(position.x, position.y);

    switch (mPlayer.state) {
        case FreeState:
            //if the player is not attached, we check whether to interact with
            queryRailAttach();
            break;

        case AttachingState:
            //the process of attaching currently takes exactly one second,
            //because the velocity vector is taken directly from the difference
            //between player's position and the closest point on the rail
            mPlayer.attachingStep++;
            //once the player is there, it stops and changes into an attached state
            if (mPlayer.attachingStep >= (1 / period / attachSpeed)) {
                mPlayer.body->SetLinearVelocity(b2Vec2_zero);
                mPlayer.pos = mPlayer.attachingPoint;
                mPlayer.state = AttachedState;
                mPlayer.touchedGround = true;
                mPlayer.rail->attachedPlayer = true;
            }
            break;

        case AttachedState:
            //in the attached state, the player moves along the rail
            queryRailAttach();
            moveOnRail();
            break;

        case DeadState:
            mPlayer.deadCounter++;
            if (mPlayer.deadCounter >= 32) {
                playerDied();
                return;
            }
            break;
    }

    if (mPlayer.pos.y() > levelH && mPlayer.state != DeadState) {
        levelFinished();
        return;
    }

    //wave generators
    for (list<WaveGenerator*>::iterator it = mWaveGens.begin(); it != mWaveGens.end(); ++it) {
        WaveGenerator *wg = *it;
        if (wg->step()) {
            float r = wg->currentRadius();
            if ((mPlayer.pos - Vec2(wg->x, wg->y)).length() < (r + playerRadius) && mPlayer.state != AttachingState) {
                bool goOn = true;
                if (mPlayer.state == AttachedState) {
                    if (!detachFromRail()) {
                        goOn = false;
                    }
                }
                if (goOn) {
                    Vec2 v = mPlayer.pos - Vec2(wg->x, wg->y);
                    v = v.toDistance((WaveGenerator::duration - wg->state) * WaveGenerator::power);
                    if (!wg->disabled) {
                        mPlayer.body->ApplyLinearImpulse(v, mPlayer.pos, true);
                        wg->disabled = true;
                    }
                }
            }
        }
    }

    //enemies
    list<Enemy*> eToDelete;
    for (list<Enemy*>::iterator it = mEnemies.begin(); it != mEnemies.end(); ++it) {
        Enemy *e = *it;
        e->pos.set(e->body->GetPosition().x, e->body->GetPosition().y);

        if (e->state == Enemy::Detached) {
            b2AABB aabb;
            aabb.lowerBound.Set(e->pos.x() - 0.7, e->pos.y() - 0.6);
            aabb.upperBound.Set(e->pos.x() + 0.7, e->pos.y() + 0.6);
            mQueriedEnemy = e;
            mBoxWorld->QueryAABB(this, aabb);
        }

        e->step(mPlayer.pos);

        if (e->pos.y() < -2) {
            eToDelete.push_back(e);
        }
    }

    for (list<Enemy*>::iterator it = eToDelete.begin(); it != eToDelete.end(); ++it) {
        mEnemies.remove(*it);
        delete *it;
    }

    //rails (only need to process deadly ones)
    for (list<Rail*>::iterator it = mRails.begin(); it != mRails.end(); ++it) {
        (*it)->step();
    }

    //gravity boost cancellation
    if (gravityBoost) {
        if (++mGravityStep > gravityBoostDuration) {
            setGravityBoost(false);
        }
    }
}

void World::pause(bool pause) {
    paused = pause;
    mLastTick = 0;
}

//fire the player in a specified direction

void World::firePlayer(float x, float y) {
    Vec2 force(x, y);
    bool detached = true;
    if (force.length() > maximumForce * 100.0f) {
        force = force.toDistance(maximumForce * 100.0f);
    }

    if (mPlayer.state == FreeState) {
        mPlayer.body->ApplyForceToCenter(force, true);
    } else if (mPlayer.state == AttachedState) {
        if (detachFromRail()) {
            mPlayer.body->ApplyForceToCenter(force, true);
        }else{
            detached = false;
        }
    }
    if (detached) {
        mPlayer.touchedGround = false;
    }
}

//in case when player's broader touch area overlaps a wave generator's narrow touch area,
//the wave generator is given the priority (because that's what the user probably
//intended to do)

bool World::preProcessTouch(float igx, float igy) {
    for (list<WaveGenerator*>::iterator it = mWaveGens.begin(); it != mWaveGens.end(); ++it) {
        WaveGenerator *wg = *it;
        float r = WaveGenerator::radius;
        if (igx >= wg->x - r && igx <= wg->x + r && igy >= wg->y - r && igy <= wg->y + r) {
            Sound::getInstance()->playSound("waveGenerator");
            wg->activate();
            return true;
        }
    }
    return false;
}

void World::touchPress(float igx, float igy) {
    for (list<WaveGenerator*>::iterator it = mWaveGens.begin(); it != mWaveGens.end(); ++it) {
        WaveGenerator *wg = *it;
        float r = WaveGenerator::touchRadius;
        if (igx >= wg->x - r && igx <= wg->x + r && igy >= wg->y - r && igy <= wg->y + r) {
            Sound::getInstance()->playSound("waveGenerator");
            wg->activate();
        }
    }
}

//look for rails in an AABB close enough to attach

void World::queryRailAttach() {
    b2AABB area;
    area.lowerBound.Set(mPlayer.pos.x() - playerRadius - attachDistance, mPlayer.pos.y() - playerRadius - attachDistance);
    area.upperBound.Set(mPlayer.pos.x() + playerRadius + attachDistance, mPlayer.pos.y() + playerRadius + attachDistance);
    mQueriedEnemy = nullptr;
    mBoxWorld->QueryAABB(this, area);
}

//all fixtures within an AABB close enough to attach

bool World::ReportFixture(b2Fixture *fixture) {
    //get the rail corresponding to the fixture
    MyUserData *ud = static_cast<MyUserData*> (fixture->GetUserData());
    if (!ud) {
        return true;
    }

    //we're gonna check every rail within the AABB
    if (ud->type == MyUserData::RailType && (mPlayer.state == FreeState || mQueriedEnemy)) {
        float shortestDistance = 9999; //initial large number for finding a minimum
        int shortestSegment = 0; //index of the closest specific segment of a rail
        b2Vec2 closestPoint; //closest point on the rail
        Rail *r = ud->rail;
        if ((r->disabled || gravityBoost) && mQueriedEnemy == nullptr) {
            return true;
        }

        //for every rail segment (or point on the starting end of that segment)
        for (unsigned int i = 0; i < r->getPoints().size() - 1; i++) {
            //calculate the distance by hand (box2d functions proved unreliable or maybe I'm just dumb)
            float distance = 0;
            Vec2 atPoint;

            const Vec2 &pt1 = r->getPoints()[i];
            const Vec2 &pt2 = r->getPoints()[i + 1];
            float len = (pt2 - pt1).length();

            Vec2 ppt1;
            Vec2 pt2pt1 = pt2 - pt1;
            if (mQueriedEnemy) {
                ppt1 = mQueriedEnemy->pos - pt1;
            }else{
                ppt1 = mPlayer.pos - pt1;
            }

            float u = ((ppt1.x() * pt2pt1.x()) + (ppt1.y() * pt2pt1.y())) / (len * len);

            if (u < 0.0) {
                atPoint = pt1;
            }else if (u > 1.0){
                atPoint = pt2;
            }else{
                atPoint = pt1 + ((pt2 - pt1) * u);
            }

            if (mQueriedEnemy) {
                distance = (atPoint - mQueriedEnemy->pos).length() - 0.5;
            }else{
                distance = (atPoint - mPlayer.pos).length() - playerRadius;
            }

            //check whether it's the shortest yet
            if (distance < shortestDistance) {
                shortestDistance = distance;
                closestPoint.Set(atPoint.x(), atPoint.y());
                shortestSegment = i;
            }
        }
        //if the player is close enough to attach, we'll attach him to that segment
        if (shortestDistance <= attachDistance) {
            if (mQueriedEnemy == nullptr) {
                if (mPlayer.state == FreeState) {
                    attachToRail(r, shortestSegment, closestPoint);
                    //don't look any further
                    return false;
                }
            } else {
                if (mQueriedEnemy->state == Enemy::Detached) {
                    if (r != mQueriedEnemy->disabledRail) {
                        mQueriedEnemy->attachToRail(r, shortestSegment, closestPoint);
                        return false;
                    }
                }
            }
        }
    } else if (ud->type == MyUserData::StaticType) {
        if (ud->staticObject->hazard) {
            const b2Transform &xa = mPlayer.body->GetTransform();
            const b2Transform &xb = ud->staticObject->body->GetTransform();
            for (unsigned int i = 0; i < ud->staticObject->getPoints().size(); i++) {
                if (b2TestOverlap(mPlayer.shape, 0, ud->staticObject->fixture->GetShape(), i, xa, xb)) {
                    killPlayer(false);
                    return false;
                }
            }
        } else if (ud->staticObject->forceField) {
            const b2Transform &xa = mPlayer.body->GetTransform();
            const b2Transform &xb = ud->staticObject->body->GetTransform();
            for (unsigned int i = 0; i < ud->staticObject->getPoints().size(); i++) {
                if (b2TestOverlap(mPlayer.shape, 0, ud->staticObject->fixture->GetShape(), i, xa, xb)) {
                    mPlayer.enterForceField();
                    return true;
                }
            }
        }
    } else if (ud->type == MyUserData::EnemyType) {
        //const b2Transform &xa = mPlayer.body->GetTransform();
        //const b2Transform &xb = ud->enemy->body->GetTransform();
        //    	if (b2TestOverlap(mPlayer.shape, 0, ud->enemy->fixture->GetShape(), 0, xa, xb)) {

        //  	}
    }
    //if the fixture doesn't belong to a rail, or the player is not close enough, keep looking
    return true;
}

void World::attachToRail(Rail *r, int segmentIndex, const b2Vec2 &nearestPoint) {
    Sound::getInstance()->playSound("railAttach");

    //stop player
    mPlayer.body->SetLinearVelocity(b2Vec2(0, 0));
    mPlayer.body->SetGravityScale(0.0f);
    mPlayer.body->SetAngularVelocity(0.0f);

    //prevent player form colliding with rails, statics and enemies
    mPlayer.setFilter(
        CollisionFilter(CollisionFilter::Player, CollisionFilter::Everything)
            .doNotCollideWith(CollisionFilter::Rail)
            .doNotCollideWith(CollisionFilter::Wall)
            .doNotCollideWith(CollisionFilter::Enemy)
            .getMask()
    );

    //set state to being in the process of attaching to rail r
    mPlayer.state = AttachingState;
    mPlayer.rail = r;
    mPlayer.railSegment = segmentIndex;
    mPlayer.attachingStep = 0;
    mPlayer.attachingPoint = nearestPoint;
    b2Vec2 speedVector = (nearestPoint - mPlayer.pos);
    speedVector *= attachSpeed;
    mPlayer.body->SetLinearVelocity(speedVector);

    //set the direction in which the player will be moving along the rail
    float distA = (mPlayer.attachingPoint - mPlayer.rail->getPoints()[mPlayer.railSegment]).length();
    for (unsigned int i = 0; i < mPlayer.railSegment; i++) {
        distA += (mPlayer.rail->getPoints()[i] - mPlayer.rail->getPoints()[i + 1]).length();
    }
    float distB = (mPlayer.attachingPoint - mPlayer.rail->getPoints()[mPlayer.railSegment + 1]).length();
    for (unsigned int i = mPlayer.railSegment + 1; i < mPlayer.rail->getPoints().size() - 1; i++) {
        distB += (mPlayer.rail->getPoints()[i] - mPlayer.rail->getPoints()[i + 1]).length();
    }
    mPlayer.direction = (distA > distB ? -1 : 1);
}

void World::moveOnRail() {
    //TODO: optimization note: this only needs to be called when the player arrives at the end of a segment
    //so just set some sort of counter to call this once it runs out
    Vec2 segmentVector = (mPlayer.rail->getPoints()[mPlayer.railSegment + 1] -
        mPlayer.rail->getPoints()[mPlayer.railSegment]).toDistance(2.0 * mPlayer.direction);
    mPlayer.body->SetLinearVelocity(segmentVector);

    unsigned int towardsPoint = mPlayer.railSegment + (mPlayer.direction == 1 ? 1 : 0);
    float dist = (mPlayer.pos - mPlayer.rail->getPoints()[towardsPoint]).length();
    if (dist <= 0.05) { //TODO: avoid skipping and floating off
        if ((mPlayer.direction == -1 && towardsPoint == 0) ||
            (mPlayer.direction == 1 && towardsPoint == mPlayer.rail->getPoints().size() - 1)) {
            mPlayer.direction = -mPlayer.direction;
            mPlayer.pos = mPlayer.rail->getPoints()[towardsPoint];
        } else {
            mPlayer.railSegment += mPlayer.direction;
            mPlayer.pos = mPlayer.rail->getPoints()[towardsPoint];
        }
    }
}

bool World::detachFromRail() {
    if (canDetach()) {
        mPlayer.state = FreeState;
        mPlayer.body->SetGravityScale(1.0f);
        mPlayer.detachCounter = 0;

        mPlayer.rail->attachedPlayer = false;
        mPlayer.rail->disableRail();
        mPlayer.setFilter(defaultFilter);
        if (gravityBoost) {
            mPlayer.setFilter(
                CollisionFilter(CollisionFilter::Player, CollisionFilter::Everything)
                    .doNotCollideWith(CollisionFilter::Rail)
                    .getMask()
            );
        }
        return true;
    }
    return false;
}

void World::calculateGrassData(Grass *grass) {
    float h = 0.4;

    Vec2 grassDiff = grass->end - grass->start;
    Vec2 unitVec = grassDiff.toDistance(h);
    float lenh = ((grassDiff.length() - 1.5f * h) / h) / 2;

    Vec2 t[4];

    Vec2::boxAroundLine(h / 2, grass->start + (unitVec * 0.5), grass->end - unitVec, t[2], t[3], t[0], t[1]);
    if (grass->end.y() > grass->start.y()) {
        swap(t[0], t[2]);
        swap(t[1], t[3]);
    }
    if (grass->reversed) {
        swap(t[0], t[2]);
        swap(t[1], t[3]);
    }

    float ydiff = h * 0.21f;
    for (int i = 0; i < 4; i++) {
        grass->pointCache[1][i * 3] = t[i].x();
        grass->pointCache[1][i * 3 + 1] = t[i].y() + ydiff;
        grass->pointCache[1][i * 3 + 2] = 0;
    }
    //swap 1,3 ; swap 2,4
    // 1    2       3      4
    //0,0, lenh,0, 0,1, lenh,1
    grass->texCache[1][0] = grass->texCache[1][4] = 0;
    grass->texCache[1][5] = grass->texCache[1][7] = 1;
    grass->texCache[1][2] = grass->texCache[1][6] = lenh;
    grass->texCache[1][1] = grass->texCache[1][3] = 0.05;
    grass->texCache[0][0] = grass->texCache[0][1] = grass->texCache[0][3] = grass->texCache[0][4] = 0;
    grass->texCache[0][2] = grass->texCache[0][6] = 1;
    grass->texCache[0][5] = grass->texCache[0][7] = 1;
    grass->texCache[2][0] = grass->texCache[2][1] = grass->texCache[2][3] = grass->texCache[2][4] = 0;
    grass->texCache[2][2] = grass->texCache[2][6] = 1;
    grass->texCache[2][5] = grass->texCache[2][7] = 1;

    //ydiff += 0.02;
    Vec2 ts1 = t[0] - (unitVec * 0.5), ts2 = t[2] - (unitVec * 0.5);
    Vec2 te1 = t[1] + unitVec, te2 = t[3] + unitVec;

    grass->pointCache[0][2] = grass->pointCache[0][5] = grass->pointCache[0][8] = grass->pointCache[0][11] = 0;
    grass->pointCache[0][0] = ts1.x();
    grass->pointCache[0][1] = ts1.y() + ydiff;
    grass->pointCache[0][3] = t[0].x();
    grass->pointCache[0][4] = t[0].y() + ydiff;
    grass->pointCache[0][6] = ts2.x();
    grass->pointCache[0][7] = ts2.y() + ydiff;
    grass->pointCache[0][9] = t[2].x();
    grass->pointCache[0][10] = t[2].y() + ydiff;

    grass->pointCache[2][2] = grass->pointCache[2][5] = grass->pointCache[2][8] = grass->pointCache[2][11] = 0;
    grass->pointCache[2][0] = t[1].x();
    grass->pointCache[2][1] = t[1].y() + ydiff;
    grass->pointCache[2][3] = te1.x();
    grass->pointCache[2][4] = te1.y() + ydiff;
    grass->pointCache[2][6] = t[3].x();
    grass->pointCache[2][7] = t[3].y() + ydiff;
    grass->pointCache[2][9] = te2.x();
    grass->pointCache[2][10] = te2.y() + ydiff;
}

void World::unloadLevel() {
    for (list<Rail*>::iterator it = mRails.begin(); it != mRails.end(); ++it) {
        mBoxWorld->DestroyBody((*it)->body);
        delete *it;
    }
    for (list<StaticObject*>::iterator it = mStatics.begin(); it != mStatics.end(); ++it) {
        mBoxWorld->DestroyBody((*it)->body);
        delete *it;
    }
    for (list<StaticObject*>::iterator it = mHazards.begin(); it != mHazards.end(); ++it) {
        mBoxWorld->DestroyBody((*it)->body);
        delete *it;
    }
    for (list<WaveGenerator*>::iterator it = mWaveGens.begin(); it != mWaveGens.end(); ++it) {
        delete *it;
    }

    for (list<Enemy*>::iterator it = mEnemies.begin(); it != mEnemies.end(); ++it) {
        mBoxWorld->DestroyBody((*it)->body);
        delete *it;
    }

    for (list<Bullet*>::iterator it = mBullets.begin(); it != mBullets.end(); ++it) {
        delete *it;
    }

    for (list<Grass*>::iterator it = mGrass.begin(); it != mGrass.end(); ++it) {
        delete *it;
    }
    mBullets.clear();
    mEnemies.clear();
    mRails.clear();
    mStatics.clear();
    mHazards.clear();
    mWaveGens.clear();
    mGrass.clear();

    delete mBoxWorld;
    mBoxWorld = nullptr;
    Log::debug("After unload");
}

void World::setGravityBoost(bool boost) {
    gravityBoost = boost;

    if (boost) {
        if ((mPlayer.state == AttachedState || mPlayer.state == AttachingState) && !canDetach()) {
            gravityBoost = false;
            return;
        }
        mBoxWorld->SetGravity(b2Vec2(0, Settings::getFloat("GravityBoost/Gravity")));
        mPlayer.fixture->SetRestitution(0);
        mPlayer.setFilter(
            CollisionFilter(CollisionFilter::Player, CollisionFilter::Everything)
                .doNotCollideWith(CollisionFilter::Rail)
                .getMask()
        );
        mGravityStep = 0;
        gbFirstHit = true;

        for (list<Enemy*>::iterator it = mEnemies.begin(); it != mEnemies.end(); ++it) {
            Enemy *e = *it;
            if (b2TestOverlap(e->fixture->GetAABB(0), mPlayer.fixture->GetAABB(0))) {
                mPlayer.touchingEnemies.insert(e);
            }
        }

        if (mPlayer.state == AttachedState || mPlayer.state == AttachingState) {
            detachFromRail();
        }
    } else {
        mBoxWorld->SetGravity(b2Vec2(0, defaultGravity));
        mPlayer.fixture->SetRestitution(Settings::getFloat("Player/Restitution"));
        mPlayer.setFilter(defaultFilter);
        mPlayer.touchingEnemies.clear();
    }
}

void World::destroyBufferObjects() {
    for (list<StaticObject*>::iterator it = mStatics.begin(); it != mStatics.end(); ++it) {
        (*it)->clearCache();
    }

    for (list<StaticObject*>::iterator it = mHazards.begin(); it != mHazards.end(); ++it) {
        (*it)->clearCache();
    }
}

void World::fireBullet(const Vec2 &origin, const Vec2 &direction) {
    Bullet *b = new Bullet;
    b->pos = origin;
    b->velocity = (direction - origin).toDistance(Enemy::bulletSpeed);
    mBullets.push_back(b);
}

void World::processBullets() {
    list<Bullet*> toDelete;
    for (list<Bullet*>::iterator it = mBullets.begin(); it != mBullets.end(); ++it) {
        Bullet *b = *it;
        b->pos = b->pos + b->velocity;
        if (b->pos.x() < 0 || b->pos.y() < 0 || b->pos.x() > levelWidth || b->pos.y() > levelH) {
            toDelete.push_back(b);
        }

        if (!gravityBoost && b->pos.x() > mPlayer.pos.x() - playerRadius && b->pos.x() < mPlayer.pos.x() + playerRadius &&
            b->pos.y() > mPlayer.pos.y() - playerRadius && b->pos.y() < mPlayer.pos.y() + playerRadius) {
            if (!gravityBoost) {
                killPlayer();
            }
        }
    }

    for (list<Bullet*>::iterator it = toDelete.begin(); it != toDelete.end(); ++it) {
        mBullets.remove(*it);
        delete *it;
    }
}

void World::killPlayer(bool stop) {
    if (invincible) {
        return;
    }

    if (mPlayer.state != DeadState) {
        mPlayer.state = DeadState;
        if (stop) {
            mPlayer.body->SetLinearVelocity(b2Vec2_zero);
            mPlayer.body->SetGravityScale(1.0f);
        }
        //mPlayer.body->ApplyForceToCenter(b2Vec2(100.0, 100.0));
        mPlayer.setFilter(CollisionFilter(CollisionFilter::Player, CollisionFilter::Nothing).getMask());
        Sound::getInstance()->playSound("death");
    }
}

void World::killEnemy(Enemy *e, bool dispose) {
    if (dispose) {
        mBoxWorld->DestroyBody(e->body);
        mEnemies.remove(e);
        delete e;
    } else {
        e->die();
    }
}

void World::BeginContact(b2Contact *contact) {
    b2Fixture *otherFixture;
    if (contact->GetFixtureA() == mPlayer.fixture) {
        otherFixture = contact->GetFixtureB();
    } else if (contact->GetFixtureB() == mPlayer.fixture) {
        otherFixture = contact->GetFixtureA();
    } else {
        return;
    }
    MyUserData *data = (MyUserData*) otherFixture->GetUserData();
    if (data->type == MyUserData::StaticType) {
        mPlayer.touchedGround = true;
        mPlayer.touching.insert(data->staticObject);
        if (gravityBoost && gbFirstHit) {
            gbFirstHit = false;
            Sound::getInstance()->playSound("groundHit");
            mVibrator.vibrate(200);
        }
    } else if (data->type == MyUserData::FloorType) {
        mPlayer.touchedGround = true;
        mPlayer.touchingFloor = true;
        if (gravityBoost && gbFirstHit) {
            gbFirstHit = false;
            Sound::getInstance()->playSound("groundHit");
            mVibrator.vibrate(200);
        }
    } else if (data->type == MyUserData::EnemyType) {
        if (gravityBoost && data->enemy->state == Enemy::Attached && mPlayer.touchingEnemies.find(data->enemy) == mPlayer.touchingEnemies.end()) {
            Sound::getInstance()->playSound("enemyHit");
            data->enemy->detach(
                CollisionFilter(CollisionFilter::Enemy)
                    .collideWith(CollisionFilter::Player)
                    .getMask()
            );
            b2Vec2 vel = mPlayer.body->GetLinearVelocity();
            vel *= 0.25f;
            mPlayer.body->SetLinearVelocity(vel);
            mVibrator.vibrate(300);
            setGravityBoost(false);
        }
    }
}

void World::EndContact(b2Contact *contact) {
    b2Fixture *otherFixture;
    if (contact->GetFixtureA() == mPlayer.fixture) {
        otherFixture = contact->GetFixtureB();
    }else if (contact->GetFixtureB() == mPlayer.fixture) {
        otherFixture = contact->GetFixtureA();
    }else{
        return;
    }
    MyUserData *data = (MyUserData*) otherFixture->GetUserData();
    if (data->type == MyUserData::StaticType) {
        mPlayer.touching.erase(data->staticObject);
    } else if (data->type == MyUserData::FloorType) {
        mPlayer.touchingFloor = false;
    }
}

bool World::canDetach() {
    for (list<StaticObject*>::iterator it = mStatics.begin(); it != mStatics.end(); ++it) {
        if (insideStaticObject(*it)) {
            return false;
        }
    }
    return true;
}

bool World::insideStaticObject(StaticObject *so) {
    for (vector<Vec2>::iterator it = so->drawPoints.begin(); it != so->drawPoints.end(); ++it) {
        const Vec2 &v1 = *it;
        ++it;
        const Vec2 &v2 = *it;
        ++it;
        const Vec2 &v3 = *it;
        const Vec2 &p = mPlayer.pos;

        bool b1 = ((p.x() - v2.x()) * (v1.y() - v2.y()) - (v1.x() - v2.x()) * (p.y() - v2.y())) < 0.0f;
        bool b2 = ((p.x() - v3.x()) * (v2.y() - v3.y()) - (v2.x() - v3.x()) * (p.y() - v3.y())) < 0.0f;
        bool b3 = ((p.x() - v1.x()) * (v3.y() - v1.y()) - (v3.x() - v1.x()) * (p.y() - v1.y())) < 0.0f;

        if ((b1 == b2) && (b2 == b3)) {
            return true;
        }
    }
    return false;
}

// -----

void World::Player::reset(bool clearBody) {
    if (clearBody) {
        body = nullptr;
        fixture = nullptr;
    }
    detachCounter = 0;
    deadCounter = 0;
    state = FreeState;
    rail = nullptr;
    touchedGround = true;
    touchingFloor = false;
    touching.clear();
    forceFieldTouching.clear();
    touchingEnemies.clear();
    if (body) {
        body->SetAngularVelocity(0.0f);
        body->SetLinearVelocity(b2Vec2_zero);
        body->SetGravityScale(1.0f);
    }
    if (fixture) {
        setFilter(defaultFilter);
    }
    inForceField = false;
}

bool World::Player::canFire() const {
    return touchedGround || touchingFloor || !touching.empty() || World::canFly || inForceField;
}

void World::Player::enterForceField() {
    body->SetGravityScale(0.05f);
    inForceField = true;
    touchedGround = true;
    b2Vec2 vel = body->GetLinearVelocity();
    vel *= 0.95f;
    body->SetLinearVelocity(vel);
}

void World::Player::leaveForceField() {
    body->SetGravityScale(1.0f);
    inForceField = false;
}

void World::Player::setFilter(int mask) {
    b2Filter filter;
    filter.maskBits = mask;
    filter.categoryBits = CollisionFilter::Player;
    fixture->SetFilterData(filter);
}

void World::render(RenderDriverInterface *renderDriver) {
    renderDriver->startWorld();
    renderDriver->drawFinishLine(this);
    renderDriver->drawFloor(this);

    // TODO: render all generic renderables in z-index order

    for (auto rail : mRails) {
        rail->render(renderDriver);
    }

    for (auto waveGenerator : mWaveGens) {
        waveGenerator->render(renderDriver);
    }

    for (auto wall : mStatics) {
        wall->render(renderDriver);
    }

    for (auto hazard : mHazards) {
        hazard->render(renderDriver);
    }

    for (auto enemy : mEnemies) {
        enemy->render(renderDriver);
    }

    for (auto bullet : mBullets) {
        bullet->render(renderDriver);
    }

    renderDriver->drawPlayer(this);
    renderDriver->endWorld();
}
