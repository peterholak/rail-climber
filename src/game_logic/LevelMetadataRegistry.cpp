#include <cstdio>
#include <cassert>
#include "platform/core.h"
#include "level_loader/XmlMetadataLoader.h"
#include "Settings.h"
#include "level_loader/YamlMetadataLoader.h"
#include "LevelMetadataRegistry.h"

using namespace std;

LevelMetadataRegistry::LevelMetadataRegistry() {
    mMetadataLoader = new YamlMetadataLoader();
    mPassedLevels = 0;
    mTotalLevels = 0;
}

LevelMetadataRegistry::~LevelMetadataRegistry() {
    for (vector<LevelMetadata*>::iterator it = mLevelData.begin(); it != mLevelData.end(); ++it) {
        delete *it;
    }
    delete mMetadataLoader;
}

const LevelMetadata * LevelMetadataRegistry::getMetadataForLevel(unsigned int levelIndex) const {
    assert(levelIndex < mLevelData.size());
    return mLevelData[levelIndex];
}

unsigned int LevelMetadataRegistry::getLevelCount() const {
    return mLevelData.size();
}

// TODO: load in constructor? RAII? do not have class in invalid state, ever?
void LevelMetadataRegistry::loadMetadataForLevels() {
    mTotalLevels = Settings::getInt("Game/Levels");

    for (unsigned int i=0; i<mTotalLevels; i++) {
        char metaFilename[1024];
        snprintf(metaFilename, 1024, "levels/level%d.yml", i + 1);
        int len;
        char *metaFile = RailClimberPlatform::loadFile(metaFilename, &len);
        LevelMetadata *level = mMetadataLoader->loadLevelMetadata(metaFile);

        char str[128];
        snprintf(str, 128, "level%d.svg", i + 1);
        level->filename = str;
        level->number = i;

        mLevelData.push_back(level);
    }
}
