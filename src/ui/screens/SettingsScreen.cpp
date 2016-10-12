#include "ui/Label.h"
#include "ui/Button.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"
#include "Settings.h"
#include "Sound.h"
#include "Translation.h"
#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer)
{
    mFullScreen = true;
    mShowTips = Settings::getSavedBool("settings", "tips");
    const Font *font = &sharedAssets.defaultFont();

    mOnButtonTextures = new Button::TextureSet(&sharedAssets.getUiTexture("on"), &sharedAssets.getUiTexture("on-down"));
    mOffButtonTextures = new Button::TextureSet(&sharedAssets.getUiTexture("off"), &sharedAssets.getUiTexture("off-down"));

    addWidget(new Label(_("Settings"), Vec2(unitConverter.horizontalCenter(), 5.8f), font, Color::black, Widget::TopCenter, Vec2(Widget::SIZE_AUTO, 0.8f)));
    Button *backButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")),
        Vec2(unitConverter.gameWidth() - 0.8f, 0.9f),
        Vec2(0.8f, Widget::SIZE_AUTO)
    );
    backButton->clicked.connect<SettingsScreen, &SettingsScreen::onBackClicked>(this);
    addWidget(backButton);

    addWidget(new Label(_("Sound effects"), Vec2(0.65f, 4.45f), font, Color::black, Widget::BottomLeft, Vec2(Widget::SIZE_AUTO, 0.7f)));
    mBtnSound = new Button(mOnButtonTextures, Vec2(unitConverter.gameWidth() - 1.3f, 4.0f), Vec2(0.9f, Widget::SIZE_AUTO));
    if (Settings::getSavedBool("settings", "sound", true) == false) {
        mBtnSound->setImageTextures(mOffButtonTextures);
    }
    mBtnSound->clicked.connect<SettingsScreen, &SettingsScreen::onSoundToggled>(this);
    addWidget(mBtnSound);

    // TODO: platform-specific can-vibrate detection
    addWidget(new Label(_("Vibrate"), Vec2(0.65f, 2.7f), font, Color::black, Widget::BottomLeft, Vec2(Widget::SIZE_AUTO, 0.7f)));
    mBtnVibrate = new Button(mOnButtonTextures, Vec2(unitConverter.gameWidth() - 1.3f, 2.3f), Vec2(0.9f, Widget::SIZE_AUTO));
    if (Settings::getSavedBool("settings", "vibrate", true) == false) {
        mBtnVibrate->setImageTextures(mOffButtonTextures);
    }
    mBtnVibrate->clicked.connect<SettingsScreen, &SettingsScreen::onVibrationToggled>(this);
    addWidget(mBtnVibrate);

    addWidget(new Label(_("Game tips"), Vec2(0.65f, /*2.35*/3.4f), font, Color::black, Widget::BottomLeft, Vec2(Widget::SIZE_AUTO, 0.7f)));
    mBtnTips = new Button(mOnButtonTextures, Vec2(unitConverter.gameWidth() - 1.3f, /*1.95*/3.0f), Vec2(0.9f, Widget::SIZE_AUTO));
    if (Settings::getSavedBool("settings", "tips", true) == false) {
        mBtnTips->setImageTextures(mOffButtonTextures);
    }
    mBtnTips->clicked.connect<SettingsScreen, &SettingsScreen::onTipsToggled>(this);
    addWidget(mBtnTips);
}

void SettingsScreen::onVibrationToggled(Button *sender) {
    /*vibrateEnabled = !vibrateEnabled;
    Settings::setSavedBool("settings", "vibrate", vibrateEnabled);
    setOnOffButton(mBtnVibrate, vibrateEnabled);*/
}

void SettingsScreen::onTipsToggled(Button *sender) {
    mShowTips = !mShowTips;
    Settings::setSavedBool("settings", "tips", mShowTips);
    setOnOffButton(mBtnTips, mShowTips);
}

void SettingsScreen::onBackClicked(Button *sender) {
    finished();
}

void SettingsScreen::onSoundToggled(Button *sender) {
    // TODO: handle change in other menus centrally, make settings have listener (signal) for changes
    Sound *sound = Sound::getInstance();
    sound->enabled = !sound->enabled;
    Settings::setSavedBool("settings", "sound", sound->enabled);
    setOnOffButton(mBtnSound, sound->enabled);
}

void SettingsScreen::setOnOffButton(Button *button, bool state) {
    if (state) {
        button->setImageTextures(mOnButtonTextures);
    }else{
        button->setImageTextures(mOffButtonTextures);
    }
}
