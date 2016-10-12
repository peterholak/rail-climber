#pragma once

#include "game_logic/Vec2.h"

class UnitConverter {
public:
    UnitConverter(int screenWidth, int screenHeight, float gameWidth, float gameHeight);

    float inGameX(int pixelX, float scrollX = 0.0f) const;
    float inGameY(int pixelY, float scrollY = 0.0f) const;
    Vec2 inGameXY(const Vec2G<int> &pixelXY, const Vec2 &scrollXY = Vec2()) const;
    float inGameLength(int pixelLength) const;
    float pixelLength(float inGameLength) const;

    float gameWidth() const { return mGameWidth; }
    float gameHeight() const { return mGameHeight; }
    float horizontalCenter() const { return mGameWidth / 2.0f; }
    float verticalCenter() const { return mGameHeight / 2.0f; }

private:
    int mScreenWidth, mScreenHeight;
    float mGameWidth, mGameHeight;
};
