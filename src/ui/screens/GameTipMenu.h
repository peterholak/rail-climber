#pragma once

#include "ui/MenuScreen.h"

class Label;
class StaticPicture;
class UnitConverter;
class SharedAssetRegistry;

class GameTipMenu : public MenuScreen {
public:
    GameTipMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer);

    static constexpr const char* sceneId = "GameTip";

private:
    StaticPicture *mTipPicture;
    Label *mTipLabel;
};
