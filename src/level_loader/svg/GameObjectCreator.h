#pragma once

class SvgPath;
class World;

class GameObjectCreator {
public:
    virtual bool matches(const SvgPath *path) = 0;
    virtual void putObjectIntoWorld(World *world, const SvgPath *path) = 0;

    virtual ~GameObjectCreator() { }
};
