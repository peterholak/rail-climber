#pragma once

#include <Box2D/Box2D.h>

class CollisionFilter {
public:
    enum Category {
        NoCollision = 0x0000,
        Player = 0x0001,
        Wall = 0x0002,
        Rail = 0x0004,
        Enemy = 0x0008,
        WaveGenerator = 0x0010
    };

    enum Mask {
        Nothing = 0x0000,
        Everything = 0xFFFF
    };

    CollisionFilter(b2Fixture *fixture);
    CollisionFilter(Category category, Mask initialMask = Nothing);

    b2Filter get();
    unsigned short getMask();
    void applyTo(b2Fixture *fixture);

    CollisionFilter &collideWith(Category category);
    CollisionFilter &doNotCollideWith(Category category);

private:
    b2Filter filter;
};
