#pragma once

#include <string>
#include <vector>
#include "ExpatParser.h"
#include "game_logic/World.h"
#include "game_logic/Enemy.h"
#include "level_loader/LevelLoaderInterface.h"

class World;
class SvgPath;
class GameObjectCreator;
class LevelMetadata;

class InkscapeSvgLevelLoader : public LevelLoaderInterface, public ExpatParser {
public:
    InkscapeSvgLevelLoader();
    ~InkscapeSvgLevelLoader();

    virtual World *loadLevel(const std::string &data, int version = 1);

    virtual void onXmlElementStart(const std::string& name, const std::map<std::string, std::string>& atts);
    virtual void onXmlElementEnd(const std::string& name);
    virtual void onXmlCharacterData(const std::string& data);

    void xmlParseRoot(const std::map<std::string, std::string> &atts);
    void xmlParseRect(const std::map<std::string, std::string> &atts);
    void xmlParsePath(const std::map<std::string, std::string> &atts);

private:
    std::vector<GameObjectCreator*> mGameObjectCreators;

    void parsePathD(SvgPath *path);
    void applyTransform(const std::string &transform, double &x, double &y);
    double coord(double num);
    double adjustY(double y);
    void unclosePath(SvgPath *path);
    void correctWinding(SvgPath *path);

    bool mHasRect;
    double mLevelWidth, mLevelHeight;
    double mDocWidth, mDocHeight;
    std::vector<SvgPath*> mPaths;
    int mLevelVersion;

    void cleanupAndMakeReadyForNextLoading();
};
