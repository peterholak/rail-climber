#include "PlayerStartPosition.h"

#include "level_loader/svg/SvgPath.h"
#include "game_logic/World.h"

bool PlayerStartPosition::matches(const SvgPath* path) {
    return (path->type == "arc" && path->style.find("fill:#00ff00") != path->style.npos);
}

void PlayerStartPosition::putObjectIntoWorld(World* world, const SvgPath* path) {
    world->getPlayer().body->SetTransform(b2Vec2(path->cx, path->cy), 0);
    world->getPlayer().pos.set(path->cx, path->cy);
}
