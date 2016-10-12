#include "InkscapeSvgLevelLoader.h"

#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <cstdio>
#include "3rdparty/poly2tri/poly2tri.h"
#include "3rdparty/clipper/clipper.hpp"
#include "platform/core.h"
#include "game_logic/LevelMetadata.h"
#include "game_logic/GameScene.h"
#include "game_logic/World.h"
#include "game_logic/Enemy.h"
#include "game_logic/Rail.h"
#include "Logging.h"
#include "level_loader/LevelLoaderException.h"
#include "level_loader/svg/SvgPath.h"

#include "level_loader/svg/GameObjectCreator.h"
#include "level_loader/svg/game_objects/EnemyCreator.h"
#include "level_loader/svg/game_objects/GrassCreator.h"
#include "level_loader/svg/game_objects/PlayerStartPosition.h"
#include "level_loader/svg/game_objects/RailCreator.h"
#include "level_loader/svg/game_objects/StaticObjectCreator.h"
#include "level_loader/svg/game_objects/WaveGeneratorCreator.h"

using namespace std;

InkscapeSvgLevelLoader::InkscapeSvgLevelLoader() : mHasRect(false) {

    // the order of these helps speed things up a bit
    // as most levels are presumably full of mostly static objects and then rails
    mGameObjectCreators.push_back(new StaticObjectCreator());
    mGameObjectCreators.push_back(new RailCreator());
    mGameObjectCreators.push_back(new EnemyCreator());
    mGameObjectCreators.push_back(new GrassCreator());
    mGameObjectCreators.push_back(new PlayerStartPosition());
    mGameObjectCreators.push_back(new WaveGeneratorCreator());
}

InkscapeSvgLevelLoader::~InkscapeSvgLevelLoader() {
    for (auto creator : mGameObjectCreators) {
        delete creator;
    }
}

World *InkscapeSvgLevelLoader::loadLevel(const string &data, int version) {
    World *w = new World();
    mLevelVersion = version;

    try {
        xmlParse(data.c_str(), data.size());
    } catch (XMLException &e) {
        throw LevelLoaderException(string("Error parsing level: ") + e.errorString());
    }

    w->levelWidth = mLevelWidth;
    w->levelH = mLevelHeight;

    w->createWorld();

    for (const SvgPath *path : mPaths) {
        for (GameObjectCreator *creator : mGameObjectCreators) {
            if (creator->matches(path)) {
                creator->putObjectIntoWorld(w, path);
                break;
            }
        }
    }

    cleanupAndMakeReadyForNextLoading();

    return w;
}

void InkscapeSvgLevelLoader::xmlParseRoot(const map<string, string>& atts) {
    const auto &width = atts.find("width");
    const auto &height = atts.find("height");

    if (width == atts.end() || height == atts.end()) {
        //TODO: xml parser cleanup
        throw LevelLoaderException("Invalid SVG: no document width or height");
    }

    //TODO: currently unused, does it even make sense to store?
    mDocWidth = coord(strtod(width->second.c_str(), nullptr));
    mDocHeight = coord(strtod(height->second.c_str(), nullptr));
}

void InkscapeSvgLevelLoader::xmlParseRect(const map<string, string>& atts) {
    if (mHasRect) {
        throw LevelLoaderException("Only one rectangle allowed in a level SVG file");
    }

    const auto &width = atts.find("width");
    const auto &height = atts.find("height");

    if (width == atts.end() || height == atts.end()) {
        throw LevelLoaderException("Invalid SVG: rect without width or height");
    }

    mLevelWidth = coord(strtod(width->second.c_str(), nullptr));
    mLevelHeight = coord(strtod(height->second.c_str(), nullptr));

    mHasRect = true;
}

void InkscapeSvgLevelLoader::xmlParsePath(const map<string, string>& atts) {
    SvgPath *path = new SvgPath();

    const auto &d = atts.find("d");
    const auto &style = atts.find("style");
    const auto &transform = atts.find("transform");

    //non-standard attributes used by inkscape
    //helpful in that we don't have to calculate it manually
    //cx, cy specify the center of a path
    //type helps identify ellipses, which have special meaning
    const auto &cx = atts.find("sodipodi:cx");
    const auto &cy = atts.find("sodipodi:cy");
    const auto &type = atts.find("sodipodi:type");

    if (d == atts.end()) {
        throw LevelLoaderException("Invalid SVG: invalid path");
    }
    path->d = d->second;

    if (transform != atts.end()) {
        path->transform = transform->second;
    }

    if (cx != atts.end() && cy != atts.end()) {
        if (path->transform.empty()) {
            path->cx = coord(strtod(cx->second.c_str(), nullptr));
            path->cy = coord(strtod(cy->second.c_str(), nullptr));
        } else {
            path->cx = strtod(cx->second.c_str(), nullptr);
            path->cy = strtod(cy->second.c_str(), nullptr);
        }

        applyTransform(path->transform, path->cx, path->cy);
        path->cy = adjustY(path->cy);
    }

    if (type != atts.end()) {
        path->type = type->second;
    }

    if (path->type.empty()) {
        parsePathD(path);
    }

    if (style != atts.end()) {
        path->style = style->second;
    } else {
        Log::warn("SVG path has no style");
    }

    mPaths.push_back(path);
}

void InkscapeSvgLevelLoader::onXmlElementStart(const string& name, const map<string, string>& atts) {
    if (name == "svg") {
        xmlParseRoot(atts);
    } else if (name == "rect") {
        xmlParseRect(atts);
    } else if (name == "path") {
        xmlParsePath(atts);
    }
}

void InkscapeSvgLevelLoader::onXmlElementEnd(const string& name) {

}

void InkscapeSvgLevelLoader::onXmlCharacterData(const string& data) {

}

void InkscapeSvgLevelLoader::applyTransform(const string &transform, double &x, double &y) {
    if (transform.empty()) {
        return;
    }

    //translation
    if (transform[0] == 't') {
        double tx, ty;
        sscanf(transform.c_str(), "translate(%lf,%lf)", &tx, &ty);
        x += coord(tx);
        y += coord(ty);

    //transformation matrix
    } else if (transform[0] == 'm') {
        double ta, tb, tc, td, te, tf;
        sscanf(transform.c_str(), "matrix(%lf,%lf,%lf,%lf,%lf,%lf)", &ta, &tb, &tc, &td, &te, &tf);
        double ox = x, oy = y;
        x = ox * coord(ta) + oy * coord(tc) + coord(te);
        y = ox * coord(tb) + oy * coord(td) + coord(tf);
    }

    // inkscape doesn't save other types of transforms into its svg files
}

/**
 * Parses a path's "d" attribute into a series of points
 * Currently only supports lines
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d
 */
void InkscapeSvgLevelLoader::parsePathD(SvgPath* path) {
    if (!path->parsed.empty()) {
        throw LevelLoaderException("parsePathD called on an already parsed path");
    }

    stringstream d(path->d, ios::in);
    char lastCmd = 0;
    double lastX = 0, lastY = 0;

    //z is the ClosePath command, not honored here
    while (!d.eof() && lastCmd != 'z') {
        string item;
        d >> item;
        //check whether the current item is a point or a command
        if ((item[0] >= '0' && item[0] <= '9') || item[0] == '-') { //a point
            //break the point string into x and y
            int pos = item.find(',');

            string strX = item.substr(0, pos), strY = item.substr(pos + 1);

            double x, y;
            if (path->transform.empty()) {
                x = coord(strtod(strX.c_str(), nullptr));
                y = coord(strtod(strY.c_str(), nullptr));
            } else {
                x = strtod(strX.c_str(), nullptr);
                y = strtod(strY.c_str(), nullptr);
            }

            //small letter moveto/lineto means relative coordinates
            //(capital letter would mean absolute)
            //convert relative coordinates to absolute
            if (lastCmd == 'm' || lastCmd == 'l') {
                x += lastX;
                y += lastY;
            }
            lastX = x;
            lastY = y;

            double pathX = x, pathY = y;
            applyTransform(path->transform, pathX, pathY);
            path->parsed.push_back(make_pair(pathX, adjustY(pathY)));
        } else { //a command
            lastCmd = item[0];
        }
    }
    unclosePath(path);

    if (path->parsed.size() > 2) {
        correctWinding(path);
    }
}

/**
 * In case first and last point of the path is the same, one has to be removed
 * This doesn't mean that the path won't be closed in-game. For polygons like walls
 * and hazards, the first and last point are always assumed to be connected
 * by a straight line.
 */
void InkscapeSvgLevelLoader::unclosePath(SvgPath* path) {
    if (path->parsed.front() == path->parsed.back()) {
        path->parsed.pop_back();
    }
}

/**
 * Converts a polygon to CCW winding if it isn't already
 */
void InkscapeSvgLevelLoader::correctWinding(SvgPath *path) {
    ClipperLib::Polygon poly;

    for (auto &pt : path->parsed) {
        ClipperLib::IntPoint p(pt.first * 10000, pt.second * 10000);
        poly.push_back(p);
    }

    bool orientation = ClipperLib::Orientation(poly);

    if (!orientation) { //CW winding
        path->parsed.reverse();
    }
}

/**
 * Covert an SVG y coordinate into an in-game y-coordinate
 * In the svg, the [0,0] point is in top left, with y growing downwards
 * In-game coordinates have origin in bottom left, with y growing upwards
 */
double InkscapeSvgLevelLoader::adjustY(double y) {
    return mLevelHeight - y;
}

/**
 * Convert from units specified inside SVG files (inkscape uses
 * user-units/pixels in there) to in-game units.
 *
 * Currently, centimeters are chosen to represent in-game units,
 * so that 100cm line in inkscape will be 100 in-game units long,
 * assuming standard inkscape setting of 90dpi.
 *
 * Since level version 2, levels are not set to SVG user-units/pixels (which would
 * directly translate to in-game units 1:1), because the resulting levels
 * were extremely tiny in inkscape, even hitting the limits for how
 * small certain objects can be. Converting from centimeters also leaves
 * more room for tiny inaccuracies in the SVG files.
 *
 * http://www.w3.org/TR/SVG/coords.html#Units
 */
double InkscapeSvgLevelLoader::coord(double num) {
    if (mLevelVersion == 1) {
        return num;
    }
    return num * 0.0282222222222;
}

void InkscapeSvgLevelLoader::cleanupAndMakeReadyForNextLoading() {
    mHasRect = false;
    for (auto path : mPaths) {
        delete path;
    }
    mPaths.clear();
}
