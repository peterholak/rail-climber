#include "Rail.h"
#include "game_logic/Enemy.h"
#include "game_logic/World.h"
#include "game_logic/CollisionFilter.h"
#include <stdarg.h>

Rail::Rail(World *w) : world(w) {
    boxCache[0] = boxCache[1] = boxCache[2] = nullptr;
    for (int i = 0; i < 6; i++) {
        aaOverdrawCache[i] = nullptr;
    }
    deadly = false;
    deadlyStage = 1;
    deadlyProgress = 0;
    deadlyDir = 0.05;
    stageTimer = 0;
    stageTimes[0] = 512;
    stageTimes[1] = 256;
    stageTimes[2] = 256;
    attachedPlayer = false;
    disabled = false;
    disabledTimer = 0;
}

Rail::~Rail() {
    for (int i = 0; i < 3; i++) {
        delete[] boxCache[i];
    }

    for (int i = 0; i < 6; i++) {
        delete[] aaOverdrawCache[i];
    }
}

void Rail::step() {
    if (deadly) {
        stageTimer++;
        if (stageTimer > stageTimes[deadlyStage - 1]) {
            deadlyStage++;
            if (deadlyStage > 3) {
                deadlyStage = 1;
            }
            stageTimer = 0;
            deadlyProgress = (deadlyStage == 3 ? 0.3 : 0);
            deadlyDir = 0.05;
        }

        if (deadlyStage == 3) {
            if (attachedPlayer) {
                world->killPlayer();
                attachedPlayer = false;
            }
            //for (list<Enemy*>::iterator it = attachedEnemies.begin(); it != attachedEnemies.end(); ++it) {
            //world->KillEnemy(*it);
            //}
            //attachedEnemies.clear();
        }

        deadlyProgress += deadlyDir;
        if (deadlyProgress > (deadlyStage == 3 ? 0.4 : 0.5) || deadlyProgress < (deadlyStage == 3 ? 0.2 : 0)) {
            deadlyDir = -deadlyDir;
        }
    }

    if (disabled) {
        disabledTimer++;
        if (disabledTimer >= World::detachTime * 32) {
            disabled = false;
            b2Filter filter = CollisionFilter(CollisionFilter::Rail, CollisionFilter::Everything).get();
            fixture->SetFilterData(filter);
        }
    }
}

void Rail::disableRail() {
    disabled = true;
    disabledTimer = 0;
    b2Filter filter = CollisionFilter(CollisionFilter::Rail, CollisionFilter::Everything)
            .doNotCollideWith(CollisionFilter::Player)
            .get();
    fixture->SetFilterData(filter);
}
