#include "UnitConverter.h"

UnitConverter::UnitConverter(int screenWidth, int screenHeight, float gameWidth, float gameHeight) :
    mScreenWidth(screenWidth), mScreenHeight(screenHeight), mGameWidth(gameWidth), mGameHeight(gameHeight)
{
    // TODO: check if horizontal to vertical ratios match
    // TODO: maybe cache ratios?
    // TODO: check rounding stuffs, maybe unit tests?
}

float UnitConverter::inGameX(int pixelX, float scrollX) const {
    return pixelX * (mGameWidth / mScreenWidth) + scrollX;
}

float UnitConverter::inGameY(int pixelY, float scrollY) const {
    return mGameHeight - pixelY * (mGameHeight / mScreenHeight) + scrollY;
}

float UnitConverter::inGameLength(int pixelLength) const {
    return (mGameHeight * ((float)pixelLength / mScreenHeight));
}

Vec2 UnitConverter::inGameXY(const Vec2G<int> &pixelXY, const Vec2 &scrollXY) const {
    return Vec2(
        inGameX(pixelXY.x(), scrollXY.x()),
        inGameY(pixelXY.y(), scrollXY.y())
    );
}

float UnitConverter::pixelLength(float inGameLength) const {
    return (inGameLength * mScreenHeight) / mGameHeight;
}
