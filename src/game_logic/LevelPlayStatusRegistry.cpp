#include <stdio.h>
#include "Settings.h"
#include <cassert>
#include "LevelPlayStatusRegistry.h"

LevelPlayStatusRegistry::LevelPlayStatusRegistry(const LevelMetadataRegistry& metadataRegistry)
    : mPassedLevelCount(0), mMetadataRegistry(metadataRegistry)
{

}

const LevelPlayStatus& LevelPlayStatusRegistry::getLevelPlayStatus(unsigned int levelNumber) const {
    assert(levelNumber < mLevels.size());
    return mLevels[levelNumber];
}

unsigned int LevelPlayStatusRegistry::getPassedLevelCount() const {
    return mPassedLevelCount;
}

void LevelPlayStatusRegistry::storeLevelPlayStatus(int levelNumber, const LevelPlayStatus &playStatus) {
    assert(playStatus.passed >= mLevels[levelNumber].passed);
    assert(playStatus.highScore <= mLevels[levelNumber].highScore);

    // maybe just return if no change detected

    char str[128];
    snprintf(str, 128, "%d_passed", levelNumber + 1);
    Settings::setSavedBool("levels", str, playStatus.passed);

    snprintf(str, 128, "%d_highscore", levelNumber + 1);
    Settings::setSavedInt("levels", str, playStatus.highScore);

    if (!mLevels[levelNumber].passed && playStatus.passed) {
        mPassedLevelCount++;
    }

    mLevels[levelNumber] = playStatus;
}

// TODO: see comment in metadata registry
void LevelPlayStatusRegistry::loadAllPlayStatuses() {
    int totalLevels = Settings::getInt("Game/Levels");
    mPassedLevelCount = 0;

    for (int i=0; i<totalLevels; i++) {
        LevelPlayStatus level;

        char str[128];
        snprintf(str, 128, "%d_passed", i + 1);
        level.passed = Settings::getSavedBool("levels", str, false);
        if (level.passed) {
            mPassedLevelCount++;
        }
        snprintf(str, 128, "%d_highscore", i + 1);
        level.highScore = Settings::getSavedInt("levels", str, LevelPlayStatus::NO_HIGH_SCORE);
        mLevels.push_back(level);
    }
}

LevelPlayStatus::Medal LevelPlayStatusRegistry::getReachedMedal(unsigned int levelNumber) const {
    auto highestMedal = LevelPlayStatus::NoMedal;

    auto &levelPlayStatus = getLevelPlayStatus(levelNumber);
    auto *levelMetadata = mMetadataRegistry.getMetadataForLevel(levelNumber);
    if (levelPlayStatus.highScore == LevelPlayStatus::NO_HIGH_SCORE) {
        return highestMedal;
    }

    for (int i = 0; i < 3; i++) {
        if (levelPlayStatus.highScore < (levelMetadata->medalTimes[i]*1000)) {
            highestMedal = (LevelPlayStatus::Medal) i;
            break;
        }
    }
    return highestMedal;
}

bool LevelPlayStatusRegistry::canPlayLevel(unsigned int levelNumber) const {
    if (getLevelPlayStatus(levelNumber).passed) {
        return true;
    }

    int passedLevels = 0;
    for (unsigned int i = 0; i <= levelNumber; i++) {
        passedLevels += getLevelPlayStatus(i).passed;
        if (i - passedLevels >= 3) {
            return false;
        }
    }
    return true;
}
