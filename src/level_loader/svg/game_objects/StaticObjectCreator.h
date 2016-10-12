#pragma once

#include "level_loader/svg/GameObjectCreator.h"
#include <list>
#include <vector>

class StaticObjectCreator : public GameObjectCreator {
public:
    virtual bool matches(const SvgPath *path);
    virtual void putObjectIntoWorld(World *world, const SvgPath *path);

private:
    void triangulate(const std::list<std::pair<double, double>> &input, std::vector<std::pair<double, double>> &output);
};
