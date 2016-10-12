#include "XmlMetadataLoader.h"
#include "game_logic/LevelMetadata.h"
#include "level_loader/LevelLoaderException.h"

using std::string;

XmlMetadataLoader::XmlMetadataLoader() {
}

XmlMetadataLoader::~XmlMetadataLoader() {
}

LevelMetadata *XmlMetadataLoader::loadLevelMetadata(const std::string& data) {
    mLevel = new LevelMetadata();
    xmlParse(data.c_str(), data.length());
    return mLevel;
}

void XmlMetadataLoader::onXmlElementStart(const std::string& name, const std::map<std::string, std::string>& atts) {
    if (name == "RailClimberLevel") {
        mLevel->background = LevelMetadata::DayBackground;
        mLevel->theme = LevelMetadata::SummerTheme;

        const auto &theme = atts.find("theme");
        const auto &background = atts.find("background");
        const auto &version = atts.find("version");

        if (theme != atts.end() && theme->second == "snow") {
            mLevel->theme = LevelMetadata::WinterTheme;
        }

        if (background != atts.end()) {
            if (background->second == "night") {
                mLevel->background = LevelMetadata::NightBackground;
            } else if (background->second == "sunset") {
                mLevel->background = LevelMetadata::SunsetBackground;
            }
        }

        if (version != atts.end()) {
            mLevel->version = strtol(version->second.c_str(), nullptr, 10);
        }
    } else if (name == "MedalTimes") {
        const auto &gold = atts.find("gold");
        const auto &silver = atts.find("silver");
        const auto &bronze = atts.find("bronze");

        //TODO: maybe this should only log a warning
        if (gold == atts.end() || silver == atts.end() || bronze == atts.end()) {
            throw LevelLoaderException("All medal times must be set");
        }

        mLevel->medalTimes[LevelMetadata::GoldMedal] = strtod(gold->second.c_str(), nullptr);
        mLevel->medalTimes[LevelMetadata::SilverMedal] = strtod(silver->second.c_str(), nullptr);
        mLevel->medalTimes[LevelMetadata::BronzeMedal] = strtod(bronze->second.c_str(), nullptr);
    } else if (name == "LevelTip") {
        const auto &type = atts.find("type");

        if (type == atts.end()) {
            throw LevelLoaderException("LevelTip with no type attribute");
        }

        mLevel->hasTip = true;
        const string& tipType = type->second;
        if (tipType == "text") {
            mLevel->tipType = LevelMetadata::TextTip;
        } else if (tipType == "picture") {
            mLevel->tipType = LevelMetadata::PictureTip;
        } else if (tipType == "animation") {
            mLevel->tipType = LevelMetadata::AnimationTip;
        } else {
            throw LevelLoaderException("Unknown tip type");
        }
    } else if (name == "Title") {
        const auto &multiline = atts.find("multiline");
        const auto &duration = atts.find("duration");

        if (multiline != atts.end() && multiline->second == "true") {
            mLevel->titleMultiLine = true;
        }

        if (duration != atts.end()) {
            mLevel->titleDuration = strtod(duration->second.c_str(), nullptr);
        }
    }
}

void XmlMetadataLoader::onXmlElementEnd(const std::string& name) {

}

void XmlMetadataLoader::onXmlCharacterData(const std::string& data) {
    if (lastElement() == "LevelTip") {
        mLevel->tipData = data;
    } else if (lastElement() == "Title") {
        mLevel->title = data;
    }
}
