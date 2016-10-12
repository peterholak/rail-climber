#include "WaveGeneratorCreator.h"
#include "level_loader/svg/SvgPath.h"
#include "game_logic/World.h"
#include "game_logic/WaveGenerator.h"

bool WaveGeneratorCreator::matches(const SvgPath* path) {
    return (
        path->type == "arc" && path->style.find("fill:#ffff00") != path->style.npos
    );
}

void WaveGeneratorCreator::putObjectIntoWorld(World* world, const SvgPath* path) {
    WaveGenerator *wg = new WaveGenerator();
    wg->x = path->cx;
    wg->y = path->cy;
    world->addWaveGenerator(wg);
}
