#pragma once

#include <string>
#include <vector>

/**
 * Represents all data about a level other than its geometry.
 */
struct LevelMetadata {
    std::string filename;
    unsigned int number; /** level number, starts from 0, so the number that the user sees is always one larger */

    int version = 1;

    enum LevelMedal {
        GoldMedal = 0,
        SilverMedal = 1,
        BronzeMedal = 2
    };

    float medalTimes[3];
    bool hasTip = false;

    enum TipType {
        PictureTip,
        TextTip,
        AnimationTip
    } tipType;

    std::string tipData;
    std::string title;
    std::vector<std::string> themeMutators;
    float titleDuration = 1.0f;
    bool titleMultiLine = false;

    enum LevelTheme {
        SummerTheme = 0,
        WinterTheme = 1
    } theme;

    enum LevelBackground {
        DayBackground = 0,
        NightBackground = 1,
        SunsetBackground = 2
    } background;

    bool hasTitle() const {
        return !title.empty();
    }
};
