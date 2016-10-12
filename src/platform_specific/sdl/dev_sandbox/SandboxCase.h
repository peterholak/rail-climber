#pragma once

#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"
#include "graphics/BitmapRenderer.h"
union SDL_Event;


class SandboxCase {
public:
    SandboxCase(UnitConverter &unitConverter, SharedAssetRegistry &sharedAssets)
        : mUnitConverter(unitConverter), mSharedAssets(sharedAssets) { }
    virtual void initializeCase() { }
    virtual void drawCase(const BitmapRenderer &bitmapRenderer) = 0;
    virtual void handleCaseEvent(const SDL_Event &event) { }
    virtual ~SandboxCase() { }

protected:
    UnitConverter &mUnitConverter;
    SharedAssetRegistry &mSharedAssets;
};