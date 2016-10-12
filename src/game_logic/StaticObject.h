#pragma once

#include "RenderDriverInterface.h"
#include "PolygonalObject.h"
#include "graphics/BufferObject.h"

class StaticObject : public PolygonalObject {
public:
    StaticObject();
    virtual ~StaticObject();

    virtual void render(RenderDriverInterface *renderDriver) { renderDriver->drawStaticObject(this); }

    // TODO: see comment at bottom, these are not really const
    void fillPointCache(float texScaling) const;
    void calculatePolygonBuffer(float texScaling, float offset = 0.6f) const;

    void clearCache();

    std::vector<Vec2> drawPoints;
    float leftmost, topmost;

    bool hazard, forceField;

    mutable bool pointCacheFilled, bufferCacheFilled;

    mutable int bufferPointCount;

    static const int POINT_CACHE_COORDINATES = 0;
    static const int POINT_CACHE_COLOR = 1;
    static const int POINT_CACHE_TEXCOORDS = 2;

    static const int BUFFER_CACHE_COORDINATES = 0;
    static const int BUFFER_CACHE_FADEOUT = 1;
    static const int BUFFER_CACHE_TEXCOORDS = 2;

    // TODO: these are basically caches, but maybe they should not be mutable -> initialized during creation and just have const object when rendering
    mutable BufferObject pointPos, pointColor, pointTex;
    mutable BufferObject bufferPos, bufferColor, bufferTex;

    static GLfloat* toGlArrayXYZ(const std::vector<Vec2>& points, GLfloat z = 0);
};
