#include "StaticObjectCreator.h"
#include "../SvgPath.h"
#include "../../../game_logic/World.h"
#include "../../../3rdparty/poly2tri/poly2tri.h"

using namespace std;

bool StaticObjectCreator::matches(const SvgPath* path) {
    return (
        path->type.empty() && (
            (path->style.find("fill:#ff00ff") != path->style.npos) ||
            (path->style.find("fill:#ff0000") != path->style.npos)
        )
    );
}

void StaticObjectCreator::putObjectIntoWorld(World* world, const SvgPath* path) {
    StaticObject *so;

    so = new StaticObject();
    if (path->style.find("fill:#ff00ff") != path->style.npos) {
        so->hazard = true;
    }

    Vec2 *points = new Vec2[path->parsed.size()];
    int i = 0;
    for (auto &pt : path->parsed) {
        points[i++].set(pt.first, pt.second);
    }

    so->create(path->parsed.size(), points);

    //TODO: draw points should be handled inside some graphics part
    vector<pair<double, double>> drawPoints; //TODO: unnecessary step
    triangulate(path->parsed, drawPoints);
    for (auto &pt : drawPoints) {
        so->drawPoints.push_back(Vec2(pt.first, pt.second));
    }

    world->addStaticObject(so);
}

/**
 * breaks a polygon into triangles using the poly2tri library
 */
void StaticObjectCreator::triangulate(const std::list<std::pair<double, double>> &input,
        std::vector<std::pair<double, double>> &output)
{
    vector<p2t::Point*> polygon;

    for (const auto &pt : input) {
        polygon.push_back(new p2t::Point(pt.first, pt.second)); //TODO: delete??
    }
    p2t::CDT cdt(polygon);
    cdt.Triangulate();
    vector<p2t::Triangle*> result = cdt.GetTriangles();

    for (auto t : result) {
        p2t::Point *pt = t->GetPoint(0);
        output.push_back(make_pair(pt->x, pt->y));
        pt = t->PointCCW(*pt);
        output.push_back(make_pair(pt->x, pt->y));
        pt = t->PointCCW(*pt);
        output.push_back(make_pair(pt->x, pt->y));
    }
}

