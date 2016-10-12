#include "RailCreator.h"
#include "level_loader/svg/SvgPath.h"
#include "game_logic/World.h"
#include "game_logic/Rail.h"

bool RailCreator::matches(const SvgPath* path) {
    return (
        path->type.empty() && (
            (path->style.find("fill:none;stroke:#000000") != path->style.npos) ||
            (path->style.find("fill:none;stroke:#00ffff") != path->style.npos)
        )
    );
}

void RailCreator::putObjectIntoWorld(World* world, const SvgPath* path) {
    Rail *rail = new Rail(world);
    if (path->style.find("fill:none;stroke:#00ffff") != path->style.npos) {
        rail->deadly = true;
    }

    Vec2 *points = new Vec2[path->parsed.size()];
    int i = 0;
    for (auto &pt : path->parsed) {
        points[i++].set(pt.first, pt.second);
    }
    rail->create(path->parsed.size(), points);

    world->addRail(rail);
}
