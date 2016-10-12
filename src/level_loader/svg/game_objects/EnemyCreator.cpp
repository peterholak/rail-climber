#include "EnemyCreator.h"
#include "level_loader/svg/SvgPath.h"
#include "game_logic/World.h"
#include "game_logic/Enemy.h"

bool EnemyCreator::matches(const SvgPath* path) {
    return (
        path->type == "arc" && (
            (path->style.find("fill:#ffffff") != std::string::npos) ||
            (path->style.find("fill:#808000") != std::string::npos)
        )
    );
}

void EnemyCreator::putObjectIntoWorld(World* world, const SvgPath* path) {
    Enemy *e = new Enemy(world);
    if (path->style.find("fill:#808000") != std::string::npos) {
        e->fast = true;
    }
    e->pos.set(path->cx, path->cy);

    world->addEnemy(e);
}
