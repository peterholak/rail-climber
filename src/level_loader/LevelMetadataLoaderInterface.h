#pragma once

#include "game_logic/LevelMetadata.h"
#include <string>
#include "LevelLoaderException.h"

class LevelMetadataLoaderInterface {
public:
    virtual LevelMetadata *loadLevelMetadata(const std::string &data) = 0;
    virtual ~LevelMetadataLoaderInterface() { }

    static LevelMetadata::TipType tipTypeFromString(std::string tipString) {
        if (tipString == "picture") {
            return LevelMetadata::PictureTip;
        }else if (tipString == "text") {
            return LevelMetadata::TextTip;
        }else if (tipString == "animation") {
            return LevelMetadata::AnimationTip;
        }
        throw LevelLoaderException("Invalid tip type");
    }
};
