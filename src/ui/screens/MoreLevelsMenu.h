#pragma once

#include "ui/MenuScreen.h"
#include "ui/Button.h"

class UnitConverter;
class SharedAssetRegistry;

class MoreLevelsMenu : public MenuScreen {
public:
    MoreLevelsMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer);
    virtual void onBackClicked(Button *sender);

    static constexpr const char* sceneId = "MoreLevels";
};
