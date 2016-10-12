#pragma once

#include "level_loader/svg/GameObjectCreator.h"

class RailCreator : public GameObjectCreator {
public:
    virtual bool matches(const SvgPath *path);
    virtual void putObjectIntoWorld(World *world, const SvgPath *path);
};
