#pragma once

#include "ui/MenuScreen.h"

class GameScene;
class Button;
class UnitConverter;
class SharedAssetRegistry;

class DeadMenu : public MenuScreen {
public:
    DeadMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer, GameScene *gameScene);

    static constexpr const char* sceneId = "Dead";

private:
    GameScene *mGameScene;

    void onRetryClicked(Button *sender);
    void onBackClicked(Button *sender);
};
