#include "PolygonalObject.h"
#include <cstdarg>

using namespace std;

PolygonalObject::PolygonalObject() : body(nullptr), fixture(nullptr) {
}

PolygonalObject::~PolygonalObject() {
}

void PolygonalObject::create(int numPoints, ...) {
    va_list argList;
    va_start(argList, numPoints);

    for (int i = 0; i < numPoints; i++) {
        float x = va_arg(argList, double), y = va_arg(argList, double);
        mPoints.push_back(Vec2(x, y));
    }

    va_end(argList);
}

void PolygonalObject::create(int numPoints, Vec2 points[]) {
    mPoints.reserve(numPoints);
    for (int i = 0; i < numPoints; i++) {
        mPoints.push_back(points[i]);
    }
}
