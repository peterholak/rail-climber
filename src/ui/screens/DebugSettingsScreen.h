#pragma once

#include "ui/MenuScreen.h"
#include "ui/Button.h"

class UnitConverter;
class SharedAssetRegistry;

class DebugSettingsScreen : public MenuScreen {
public:
    DebugSettingsScreen(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer);

    static constexpr const char* sceneId = "DebugSettings";

private:
    void setOnOffButton(Button *button, bool state);

    Button *mFpsButton;
    Button *mBackgroundButton;

    Button::TextureSet *mOnButtonTextures;
    Button::TextureSet *mOffButtonTextures;

    bool mShowFps = false;
    bool mShowBackground = false;

    void onBackClicked(Button *sender);
    void onFpsToggled(Button *sender);
    void onBackgroundToggled(Button *sender);
};
