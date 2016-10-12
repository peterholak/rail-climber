#include "Enemy.h"
#include "game_logic/World.h"
#include "game_logic/CollisionFilter.h"

float Enemy::halfWidth = 0.5;
float Enemy::halfHeight = 0.45;
float Enemy::firingRange = 10.0;
int Enemy::firingDelay = 128;
int Enemy::mouthDelay = 60;
float Enemy::bulletSpeed = 0.05f;
float Enemy::bulletSize = 0.3f;

#define PI 3.141592654
#define PI2 1.570796327

Enemy::Enemy(World *w) : body(nullptr), fixture(nullptr), eyePosition(0), mouthOpen(false),
firingCounter(-mouthDelay), closeMouthCounter(0), active(false), state(Detached), rail(nullptr),
railSegment(0), attachingStep(0), targetAngle(0), angleCounter(0), direction(0), disabledRail(nullptr), disabledTimer(0),
mWorld(w), fast(false) {
}

Enemy::~Enemy() {

}

void Enemy::step(const Vec2 &playerPos) {
    setEyePosition(playerPos);

    switch (state) {
        case Detached:
            if (disabledRail) {
                disabledTimer++;
                if (disabledTimer > 128) {
                    disabledRail = nullptr;
                }
            }
            break;

        case Attaching:
            attachingStep++;
            if (attachingStep >= (1 / 0.016f / World::attachSpeed)) {
                body->SetLinearVelocity(b2Vec2_zero);
                body->SetAngularVelocity(0);
                pos = attachingPoint;
                state = Attached;
                b2Filter filter = CollisionFilter(CollisionFilter::Enemy)
                        .collideWith(CollisionFilter::Player)
                        .get();
                fixture->SetFilterData(filter);
                //rail->attachedEnemies.push_back(this);
            }
            break;

        case Attached:
        {
            //in the attached state, the player moves along the rail
            moveOnRail();
            Vec2 plDist = pos - playerPos;
            if (mouthOpen) {
                closeMouthCounter++;
                if (closeMouthCounter > mouthDelay) {
                    closeMouthCounter = 0;
                    mouthOpen = false;
                }
            }

            if (plDist.length() < firingRange) {
                int activeFiringDelay = firingDelay;
                if (fast) {
                    activeFiringDelay = firingDelay / 2;
                }
                firingCounter++;
                if (firingCounter > activeFiringDelay - mouthDelay / 2) {
                    mouthOpen = true;
                }
                if (firingCounter > activeFiringDelay) {
                    mWorld->fireBullet(pos, playerPos);
                    firingCounter = -mouthDelay;
                }
            } else {
                firingCounter = 0;
            }
            break;
        }

        case Dying:
            if (pos.y() < 0) {
                mWorld->killEnemy(this, true); //this deletes this object
                return;
            }
            break;
    }
}

void Enemy::moveOnRail() {
    float espd = 2.0f;
    if (fast) {
        espd = 3.4f;
    }
    Vec2 segmentVector = (rail->getPoints()[railSegment + 1] - rail->getPoints()[railSegment]).toDistance(espd * direction);
    body->SetLinearVelocity(segmentVector);

    angleCounter++;
    if (angleCounter > 32) {
        body->SetAngularVelocity(0);
    }

    Vec2 railPart = rail->getPoints()[railSegment] - rail->getPoints()[railSegment + 1];
    Vec2 enemyDiff = railPart.perpendicular(true).toDistance(Enemy::halfHeight - 0.1);
    unsigned int towardsPoint = railSegment + (direction == 1 ? 1 : 0);
    float dist = (pos - (rail->getPoints()[towardsPoint] + enemyDiff)).length();
    if (dist <= 0.05) { //TODO: avoid skipping and floating off
        if ((direction == -1 && towardsPoint == 0) || (direction == 1 && towardsPoint == rail->getPoints().size() - 1)) {
            direction = -direction;
            pos = rail->getPoints()[towardsPoint];
        } else {
            railSegment += direction;
            pos = rail->getPoints()[towardsPoint];
        }
        railPart = rail->getPoints()[towardsPoint] - rail->getPoints()[towardsPoint + direction];
        Vec2 enemyDiff = railPart.perpendicular(direction == 1).toDistance(Enemy::halfHeight - 0.1);
        pos = pos + enemyDiff;
        float angle = normalizeAngle(railPart.angle());
        body->SetTransform(pos, normalizeAngle(body->GetAngle()));
        if (direction == 0) {
            targetAngle = (angle - body->GetAngle()) * World::attachSpeed;
        }else{
            targetAngle = (angle - body->GetAngle()) * World::attachSpeed;
        }
        body->SetAngularVelocity(targetAngle);
        angleCounter = 0;
    }
}

void Enemy::attachToRail(Rail *r, int segmentIndex, const b2Vec2 &nearestPoint) {
    if (r == disabledRail) {
        return;
    }

    //stop enemy
    body->SetLinearVelocity(b2Vec2_zero);
    body->SetGravityScale(0.0f);
    body->SetAngularVelocity(0.0f);

    //prevent enemy form colliding with rails
    b2Filter filter = CollisionFilter(CollisionFilter::Enemy, CollisionFilter::Nothing).get();
    fixture->SetFilterData(filter);

    //set state to being in the process of attaching to rail r
    state = Attaching;
    rail = r;
    disabledRail = nullptr;
    railSegment = segmentIndex;
    Vec2 railPart = rail->getPoints()[railSegment] - rail->getPoints()[railSegment + 1];
    attachingStep = 0;
    attachingPoint = nearestPoint + railPart.perpendicular(true).toDistance(Enemy::halfHeight - 0.1);
    b2Vec2 speedVector = (attachingPoint - pos);
    speedVector *= World::attachSpeed;
    body->SetLinearVelocity(speedVector);

    float railAngle = normalizeAngle(railPart.angle());
    float myAngle = normalizeAngle(body->GetAngle());
    body->SetAngularVelocity((railAngle - myAngle) * World::attachSpeed);

    //set the direction in which the enemy will be moving along the rail
    assert(railSegment < rail->getPoints().size() - 1);
    float distA = (attachingPoint - rail->getPoints()[railSegment]).length();
    float distB = (attachingPoint - rail->getPoints()[railSegment + 1]).length();
    direction = (distA < distB ? -1 : 1);
}

float Enemy::normalizeAngle(float angle) {
    if (angle < 0) {
        while (angle < -PI2) {
            angle += PI;
        }
    } else {
        while (angle > PI2) {
            angle -= PI;
        }
    }
    return angle;
}

void Enemy::setEyePosition(const Vec2 &playerPos) {
    if (pos.x() < playerPos.x()) {
        eyePosition = (pos.y() < playerPos.y() ? 0 : 1);
    }else{
        eyePosition = (pos.y() < playerPos.y() ? 3 : 2);
    }
}

void Enemy::die() {
    if (state == Attached) {
        //rail->attachedEnemies.remove(this);
    }
    state = Dying;
    body->ApplyForceToCenter(b2Vec2(10, 10), true);
    body->SetGravityScale(1.0f);
    b2Filter nullFilter = CollisionFilter(CollisionFilter::Enemy, CollisionFilter::Nothing).get();
    fixture->SetFilterData(nullFilter);
}

void Enemy::detach(int filterMask) {
    if (state != Detached && state != Dying) {
        disabledRail = rail;
        disabledTimer = 0;
        state = Detached;
        body->SetGravityScale(1.0f);
        b2Filter filter;
        filter.categoryBits = CollisionFilter::Enemy;
        filter.maskBits = filterMask;
        fixture->SetFilterData(filter);
    }
}
