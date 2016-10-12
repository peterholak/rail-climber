#pragma once

#include "LevelMetadataLoaderInterface.h"

class YamlMetadataLoader : public LevelMetadataLoaderInterface {
public:
    virtual LevelMetadata *loadLevelMetadata(const std::string &data);
};


