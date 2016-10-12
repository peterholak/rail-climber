#pragma once

#include "game_logic/World.h"

class UnitConverter;
class SharedAssetRegistry;
class World;
class Theme;

class WorldRenderer {
public:
    WorldRenderer(const UnitConverter &unitConverter, const SharedAssetRegistry &assetRegistry);
    void drawPlayer(const World *world, Theme *theme);
    void drawFinishLine(const World *world, Theme *theme);
    void drawFloor(const World *world, Theme *theme);

private:
    const UnitConverter &mUnitConverter;
    const SharedAssetRegistry &mAssetRegistry;

public:
    static int playerTextureId;
    static int playerInactiveTextureId;
    static int playerGlareTextureId;
    static int finishTextureId;
    static int floorTextureId;
};


