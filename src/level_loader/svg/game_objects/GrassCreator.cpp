#include "GrassCreator.h"
#include "level_loader/svg/SvgPath.h"
#include "game_logic/World.h"

bool GrassCreator::matches(const SvgPath* path) {
    return (
        path->type.empty() && (
            (path->style.find("fill:none;stroke:#00ff00") != path->style.npos) ||
            (path->style.find("fill:none;stroke:#00ee00") != path->style.npos)
        )
    );
}

void GrassCreator::putObjectIntoWorld(World* world, const SvgPath* path) {
    World::Grass *grass = new World::Grass();
    if (path->style.find("fill:none;stroke:#00ee00") != path->style.npos) {
        grass->reversed = true;
    }else{
        grass->reversed = false;
    }
    grass->start.set(path->parsed.front().first, path->parsed.front().second);
    grass->end.set(path->parsed.back().first, path->parsed.back().second);
    world->addGrass(grass);
}
