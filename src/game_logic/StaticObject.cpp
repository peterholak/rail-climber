#include "StaticObject.h"
#include "3rdparty/clipper/clipper.hpp"
#include "Logging.h"
#include <limits>

using namespace std;

StaticObject::StaticObject() : leftmost(0), topmost(0), hazard(0), bufferPointCount(0) {
    forceField = false;
    pointCacheFilled = bufferCacheFilled = false;
}

StaticObject::~StaticObject() {
    Log::debug("Destructor on %p", this);
    clearCache();
}

void StaticObject::clearCache() {
    pointCacheFilled = bufferCacheFilled = false;
    pointPos.unload();
    pointColor.unload();
    pointTex.unload();
    bufferPos.unload();
    bufferColor.unload();
    bufferTex.unload();
}

void StaticObject::fillPointCache(float texScaling) const {
    int nPoints = drawPoints.size();
    if (!pointCacheFilled) {
        GLfloat *pts = new float[nPoints * 3];
        int i = 0;
        for (vector<Vec2>::const_iterator it = drawPoints.begin(); it != drawPoints.end(); ++it) {
            pts[i++] = (*it).x();
            pts[i++] = (*it).y();
            pts[i++] = 0;
        }
        pointPos.create(pts, nPoints * 3);
        delete[] pts;

        GLfloat *color = new float[nPoints * 4];
        for (i = 0; i < nPoints * 4; i++) {
            color[i] = 1;
        }
        pointColor.create(color, nPoints * 4);
        delete[] color;

        GLfloat *texcoord = new float[nPoints * 2];
        i = 0;
        for (vector<Vec2>::const_iterator it = drawPoints.begin(); it != drawPoints.end(); ++it) {
            float x = (*it).x(), y = (*it).y();

            texcoord[i++] = (x - leftmost) * texScaling;
            texcoord[i++] = (y - topmost) * texScaling;
        }
        pointTex.create(texcoord, nPoints * 2);
        delete[] texcoord;
        pointCacheFilled = true;
    }
}

void StaticObject::calculatePolygonBuffer(float texScaling, float offset) const {
    if (bufferCacheFilled) {
        return;
    }

    int bottommostIndex = 0;
    float bottommost = std::numeric_limits<float>::max();
    unsigned int j = 0;
    for (vector<Vec2>::const_iterator it = mPoints.begin(); it != mPoints.end(); ++it, ++j) {
        const Vec2 &point = *it;
        if (point.y() < bottommost) {
            bottommostIndex = j;
            bottommost = point.y();
        }
    }

    ClipperLib::Polygons origPolys;
    ClipperLib::Polygons resultPolys;

    ClipperLib::Polygon source;
    ClipperLib::IntPoint ip;
    for (vector<Vec2>::const_iterator it = mPoints.begin(); it != mPoints.end(); ++it) {
        const Vec2 &point = *it;
        ip.X = point.x() * 10000;
        ip.Y = point.y() * 10000;
        source.push_back(ip);
    }
    origPolys.push_back(source);

    vector<Vec2> outPoly;
    map<int, bool> squared;
    ClipperLib::OffsetPolygons(origPolys, resultPolys, offset * 10000, ClipperLib::jtMiter, 100, true, &squared);

    ClipperLib::Polygon result = resultPolys.front();

    for (vector<ClipperLib::IntPoint>::iterator it = result.begin(); it != result.end(); ++it) {
        Vec2 resPt((*it).X / 10000.0, (*it).Y / 10000.0);
        outPoly.push_back(resPt);
    }

    unsigned int i = 0, sqOff = 0;
    vector<Vec2> triangles;
    vector<GLfloat> fadeValues; //inner points have 1, outer have 0, to achieve a fade-out effect

    for (unsigned int k = 0; k < mPoints.size(); k++, i++) {
        Vec2 current = mPoints[(bottommostIndex + k) % mPoints.size()];
        Vec2 next = mPoints[(bottommostIndex + k + 1) % mPoints.size()];

        if (!squared[i]) {
            Vec2 currentOut = outPoly[(i + sqOff) % outPoly.size()];
            Vec2 nextOut = outPoly[(i + sqOff + 1) % outPoly.size()];

            triangles.push_back(current);
            triangles.push_back(currentOut);
            triangles.push_back(nextOut);

            fadeValues.push_back(1);
            fadeValues.push_back(0);
            fadeValues.push_back(0);

            triangles.push_back(current);
            triangles.push_back(next);
            triangles.push_back(nextOut);

            fadeValues.push_back(1);
            fadeValues.push_back(1);
            fadeValues.push_back(0);
        } else {
            Vec2 currentOutA = outPoly[(i + sqOff) % outPoly.size()];
            Vec2 currentOutB = outPoly[(i + sqOff + 1) % outPoly.size()];
            Vec2 nextOut = outPoly[(i + sqOff + 2) % outPoly.size()];

            //"extra" triangle
            triangles.push_back(currentOutA);
            triangles.push_back(currentOutB);
            triangles.push_back(current);

            fadeValues.push_back(0);
            fadeValues.push_back(0);
            fadeValues.push_back(1);

            triangles.push_back(current);
            triangles.push_back(currentOutB);
            triangles.push_back(nextOut);

            fadeValues.push_back(1);
            fadeValues.push_back(0);
            fadeValues.push_back(0);

            triangles.push_back(current);
            triangles.push_back(next);
            triangles.push_back(nextOut);

            fadeValues.push_back(1);
            fadeValues.push_back(1);
            fadeValues.push_back(0);

            sqOff++;
        }
    }

    GLfloat *pts = toGlArrayXYZ(triangles);
    GLfloat *fadeout = new GLfloat[fadeValues.size()*4];
    GLfloat *texcoord = new GLfloat[triangles.size()*2];

    for (unsigned int i = 0; i < fadeValues.size(); i++) {
        fadeout[i * 4 ] = 1;
        fadeout[i * 4 + 1] = 1;
        fadeout[i * 4 + 2] = 1;
        fadeout[i * 4 + 3] = fadeValues[i];
    }

    for (unsigned int i = 0; i < triangles.size(); i++) {
        float x = triangles[i].x(), y = triangles[i].y();
        texcoord[i * 2 ] = (x - leftmost) * texScaling;
        texcoord[i * 2 + 1] = (y - topmost) * texScaling;
    }
    bufferPointCount = triangles.size();

    bufferPos.create(pts, triangles.size()*3);
    bufferColor.create(fadeout, fadeValues.size()*4);
    bufferTex.create(texcoord, triangles.size()*2);

    delete[] pts;
    delete[] fadeout;
    delete[] texcoord;

    bufferCacheFilled = true;
}

GLfloat *StaticObject::toGlArrayXYZ(const std::vector<Vec2> &points, GLfloat z) {
    GLfloat *arr = new GLfloat[points.size() * 3];
    int i = 0;
    for (vector<Vec2>::const_iterator it = points.begin(); it != points.end(); ++it) {
        arr[i++] = it->x();
        arr[i++] = it->y();
        arr[i++] = z;
    }
    return arr;
}
