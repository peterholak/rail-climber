#pragma once

#include <climits>

struct LevelPlayStatus {
    static constexpr int NO_HIGH_SCORE = INT_MAX;

    bool passed = false;
    int highScore = NO_HIGH_SCORE;

    enum Medal {
        NoMedal = 3, BronzeMedal = 2, SilverMedal = 1, GoldMedal = 0
    };

    static constexpr const char *medalToString(Medal medal) {
        return (
            medal == NoMedal ? "none" : (
            medal == BronzeMedal ? "bronze" : (
            medal == SilverMedal ? "silver" : (
            medal == GoldMedal ? "gold" :
            "invalid"
        ))));
    }
};
