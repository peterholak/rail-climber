#include "level_loader/YamlMetadataLoader.h"
#include "platform/core.h"
#include "YamlLevelCase.h"

void YamlLevelCase::initializeCase() {
    YamlMetadataLoader *loader = new YamlMetadataLoader();
    auto metadata = loader->loadLevelMetadata(RailClimberPlatform::loadFile("levels/level1.yml"));


}
