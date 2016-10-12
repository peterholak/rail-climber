#pragma once

#include "ui/MenuScreen.h"
#include "graphics/SharedAssetRegistry.h"

class Button;

class MainMenuScreen : public MenuScreen {
public:
    MainMenuScreen(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer);

    static constexpr const char* sceneId = "MainMenu";

private:
    void onPlayClicked(Button *sender);
    void onSettingsClicked(Button *sender);
    void onQuitClicked(Button *sender);
    void onDebugClicked(Button *sender);
};
