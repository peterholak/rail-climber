#pragma once

#include "RenderDriverInterface.h"
#include "PolygonalObject.h"

class WaveGenerator : public Renderable {
public:
    WaveGenerator();
    virtual ~WaveGenerator();

    virtual void render(RenderDriverInterface *renderDriver) { renderDriver->drawWaveGenerator(this); }

    void activate();
    bool step();
    float currentRadius() const;

    float x, y;
    bool active;
    float state;

    bool disabled;

    static float radius;
    static float touchRadius;
    static float maxRadius;
    static int duration;
    static float power;
};
