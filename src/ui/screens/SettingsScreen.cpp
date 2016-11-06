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

    auto titleLabel = addWidget(
        new Label(
            _("Settings"),
            Vec2(unitConverter.horizontalCenter(), 6.2f),
            font,
            Color::black,
            Widget::TopCenter,
            Vec2(Widget::SIZE_AUTO, 0.8f)
        )
    );
    Button *backButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")),
        Vec2(unitConverter.gameWidth() - 0.8f, 0.9f),
        Vec2(0.8f, Widget::SIZE_AUTO)
    );
    backButton->clicked.connect<SettingsScreen, &SettingsScreen::onBackClicked>(this);
    addWidget(backButton);

    auto buttonSize = Vec2(0.9f, 0.9f);

    auto soundLabel = addWidget(
        new Label(
            _("Sound effects"),
            Vec2(0.65f, 4.45f),
            font,
            Color::black,
            Widget::BottomLeft,
            Vec2(Widget::SIZE_AUTO, 0.7f)
        )
    );
    mBtnSound = addWidget(
        new Button(
            mOnButtonTextures,
            alignWithY(unitConverter.gameWidth() - 1.3, soundLabel, buttonSize),
            buttonSize
        )
    );
    if (Settings::getSavedBool("settings", "sound", true) == false) {
        mBtnSound->setImageTextures(mOffButtonTextures);
    }
    mBtnSound->clicked.connect<SettingsScreen, &SettingsScreen::onSoundToggled>(this);

    // TODO: platform-specific can-vibrate detection
    auto vibrateLabel = addWidget(
        new Label(
            _("Vibrate"),
            below(soundLabel),
            font,
            Color::black,
            Widget::BottomLeft,
            matchHeight(soundLabel)
        )
    );
    mBtnVibrate = addWidget(
        new Button(
            mOnButtonTextures,
            alignWithYx(vibrateLabel, mBtnSound, buttonSize),
            buttonSize
        )
    );
    if (Settings::getSavedBool("settings", "vibrate", true) == false) {
        mBtnVibrate->setImageTextures(mOffButtonTextures);
    }
    mBtnVibrate->clicked.connect<SettingsScreen, &SettingsScreen::onVibrationToggled>(this);

    auto gameTipsLabel = addWidget(
        new Label(
            _("Game tips"),
            below(vibrateLabel),
            font,
            Color::black,
            Widget::BottomLeft,
            matchHeight(vibrateLabel)
        )
    );

    mBtnTips = addWidget(
        new Button(
            mOnButtonTextures,
            alignWithYx(gameTipsLabel, mBtnVibrate, buttonSize),
            buttonSize
        )
    );

    if (Settings::getSavedBool("settings", "tips", true) == false) {
        mBtnTips->setImageTextures(mOffButtonTextures);
    }

    mBtnTips->clicked.connect<SettingsScreen, &SettingsScreen::onTipsToggled>(this);
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
