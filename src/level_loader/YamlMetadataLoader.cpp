#include "YamlMetadataLoader.h"
#include <yaml-cpp/yaml.h>

using namespace std;

LevelMetadata *YamlMetadataLoader::loadLevelMetadata(const std::string &data) {
    LevelMetadata *metadata = new LevelMetadata();
    YAML::Node levelYml = YAML::Load(data.c_str());

    metadata->version = levelYml["version"].as<int>();

    if (levelYml["title"]) {
        if (levelYml["title"]["duration"]) {
            metadata->titleDuration = levelYml["title"]["duration"].as<float>();
        }
        metadata->title = levelYml["title"]["text"].as<string>();

        if (levelYml["title"]["multiline"]) {
            metadata->titleMultiLine = levelYml["title"]["multiline"].as<bool>();
        }
    }

    metadata->medalTimes[LevelMetadata::GoldMedal] = levelYml["medalTimes"]["gold"].as<float>();
    metadata->medalTimes[LevelMetadata::SilverMedal] = levelYml["medalTimes"]["silver"].as<float>();
    metadata->medalTimes[LevelMetadata::BronzeMedal] = levelYml["medalTimes"]["bronze"].as<float>();

    if (levelYml["tip"]) {
        metadata->tipType = tipTypeFromString(levelYml["tip"]["type"].as<string>());
        if (metadata->tipType == LevelMetadata::TextTip) {
            metadata->tipData = levelYml["tip"]["message"].as<string>();
        }else if (metadata->tipType == LevelMetadata::PictureTip) {
            metadata->tipData = levelYml["tip"]["picture"].as<string>();
        }
        // TODO: specify which animation in config file and not hard-coded to level number
    }

    if (levelYml["themeMutators"]) {
        metadata->themeMutators = levelYml["themeMutators"].as<vector<string>>();
    }
    return metadata;
}
