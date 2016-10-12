#pragma once

#include <vector>
#include <algorithm>
#include "game_logic/Vec2.h"
#include "Renderable.h"
#include <Box2D/Box2D.h>

class PolygonalObject : public Renderable {
public:
    PolygonalObject();
    virtual ~PolygonalObject();

    b2Body *body;
    b2Fixture *fixture;

    void create(int numPoints, ...);
    void create(int numPoints, Vec2 points[]);

    const std::vector<Vec2>& getPoints() const {
        return mPoints;
    }

protected:
    std::vector<Vec2> mPoints;
};
