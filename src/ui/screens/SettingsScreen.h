#pragma once

#include "ui/MenuScreen.h"
#include "graphics/SharedAssetRegistry.h"
#include "ui/Button.h"

class UnitConverter;
class SharedAssetRegistry;

class SettingsScreen : public MenuScreen {
public:
    SettingsScreen(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer);

    static constexpr const char* sceneId = "Settings";

private:
    Button *mBtnSound;
    Button *mBtnVibrate;
    Button *mBtnTips;
    bool mShowTips;

    void onSoundToggled(Button *sender);
    void setOnOffButton(Button *button, bool state);

    Button::TextureSet *mOnButtonTextures;
    Button::TextureSet *mOffButtonTextures;

    void onVibrationToggled(Button *sender);
    void onTipsToggled(Button *sender);
    void onBackClicked(Button *sender);
};
