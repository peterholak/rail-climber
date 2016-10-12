#pragma once

#include <vector>
#include "level_loader/LevelMetadataLoaderInterface.h"
#include "LevelMetadata.h"

class LevelMetadataRegistry {
public:
    LevelMetadataRegistry();
    ~LevelMetadataRegistry();

    void loadMetadataForLevels();
    unsigned int getLevelCount() const;
    const LevelMetadata * getMetadataForLevel(unsigned int levelIndex) const;

private:
    std::vector<LevelMetadata*> mLevelData;
    LevelMetadataLoaderInterface *mMetadataLoader;
    unsigned int mTotalLevels;
    unsigned int mPassedLevels;
};
