#pragma once

#include "LevelPlayStatus.h"
#include "LevelMetadataRegistry.h"
#include <vector>

class LevelPlayStatusRegistry {
public:
    LevelPlayStatusRegistry(const LevelMetadataRegistry &metadataRegistry);
    
    const LevelPlayStatus& getLevelPlayStatus(unsigned int levelNumber) const;
    unsigned int getPassedLevelCount() const;
    void storeLevelPlayStatus(int levelNumber, const LevelPlayStatus &playStatus);

    void loadAllPlayStatuses();

    LevelPlayStatus::Medal getReachedMedal(unsigned int levelNumber) const;
    bool canPlayLevel(unsigned int levelNumber) const;

private:
    const LevelMetadataRegistry &mMetadataRegistry;
    unsigned int mPassedLevelCount;
    std::vector<LevelPlayStatus> mLevels;
};
