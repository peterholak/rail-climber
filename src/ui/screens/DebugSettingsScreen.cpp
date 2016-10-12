#include "DebugSettingsScreen.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

DebugSettingsScreen::DebugSettingsScreen(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer)
{
    mFullScreen = true;
    const Font *font = &sharedAssets.defaultFont();

    mOnButtonTextures = new Button::TextureSet(&sharedAssets.getUiTexture("on"), &sharedAssets.getUiTexture("on-down"));
    mOffButtonTextures = new Button::TextureSet(&sharedAssets.getUiTexture("off"), &sharedAssets.getUiTexture("off-down"));

    addWidget(new Label("FPS", Vec2(0.65, 4.45), font, Color::black, Widget::BottomLeft, Vec2(Widget::SIZE_AUTO, 0.7)));
    addWidget(new Label("Background", Vec2(0.65, 3.4), font, Color::black, Widget::BottomLeft, Vec2(Widget::SIZE_AUTO, 0.7)));
    addWidget(new Label("Compress textures", Vec2(0.65, 2.35), font, Color::black, Widget::BottomLeft, Vec2(Widget::SIZE_AUTO, 0.7)));

    Button *backButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")),
        Vec2(unitConverter.gameWidth() - 0.8f, 0.9f),
        Vec2(0.8f, Widget::SIZE_AUTO)
    );
    backButton->clicked.connect<DebugSettingsScreen, &DebugSettingsScreen::onBackClicked>(this);
    addWidget(backButton);

    mFpsButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("on"), &sharedAssets.getUiTexture("on-down")),
        Vec2(unitConverter.gameWidth() - 1.3f, 4.0f),
        Vec2(0.9f, Widget::SIZE_AUTO)
    );
    setOnOffButton(mFpsButton, mShowFps);
    mFpsButton->clicked.connect<DebugSettingsScreen, &DebugSettingsScreen::onFpsToggled>(this);
    addWidget(mFpsButton);

    mBackgroundButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("on"), &sharedAssets.getUiTexture("on-down")),
        Vec2(unitConverter.gameWidth() - 1.3f, 3.0f),
        Vec2(0.9f, Widget::SIZE_AUTO)
    );
    mBackgroundButton->clicked.connect<DebugSettingsScreen, &DebugSettingsScreen::onBackgroundToggled>(this);
    addWidget(mBackgroundButton);
}

void DebugSettingsScreen::setOnOffButton(Button *button, bool state) {
    if (state) {
        button->setImageTextures(mOnButtonTextures);
    }else{
        button->setImageTextures(mOffButtonTextures);
    }
}

void DebugSettingsScreen::onBackClicked(Button *sender) {
    finished();
}

void DebugSettingsScreen::onFpsToggled(Button *sender) {
    mShowFps = !mShowFps;
    setOnOffButton(mFpsButton, mShowFps);
}

void DebugSettingsScreen::onBackgroundToggled(Button *sender) {
    mShowBackground = !mShowBackground;
    setOnOffButton(mBackgroundButton, mShowBackground);
}
