#include "Settings.h"
#include "ui/Button.h"
#include "Translation.h"
#include "graphics/SharedAssetRegistry.h"
#include "game_logic/UnitConverter.h"
#include "MainMenuScreen.h"
#include "SettingsScreen.h"
#include "LevelSelectScreen.h"
#include "DebugSettingsScreen.h"

MainMenuScreen::MainMenuScreen(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer)
{
    mFullScreen = true;

    bool hasQuit = Settings::getBool("Graphics/QuitButton");
    bool hasDebug = Settings::getBool("Graphics/DebugButton");

    float hasQuitOffset = -(!hasQuit) * 0.4f;

    Button *playButton = new Button(
        new Button::OwnedTextureSet(_("pictures/ui/play.png"), _("pictures/ui/play-down.png")),
        Vec2(unitConverter.horizontalCenter(),  5.0f + hasQuitOffset),
        Vec2(Widget::SIZE_AUTO, 0.7),
        Widget::Center
    );
    playButton->clicked.connect<MainMenuScreen, &MainMenuScreen::onPlayClicked>(this);
    addWidget(playButton);

    Button *settingsButton = new Button(
        new Button::OwnedTextureSet(_("pictures/ui/settings.png"), _("pictures/ui/settings-down.png")),
        Vec2(unitConverter.horizontalCenter(), 3.8f + hasQuitOffset),
        Vec2(Widget::SIZE_AUTO, 0.7),
        Widget::Center
    );
    settingsButton->clicked.connect<MainMenuScreen, &MainMenuScreen::onSettingsClicked>(this);
    addWidget(settingsButton);

    if (hasQuit) {
        Button *quitButton = new Button(
            new Button::OwnedTextureSet(_("pictures/ui/end.png"), _("pictures/ui/end-down.png")),
            Vec2(unitConverter.horizontalCenter(), 2.6f + hasQuitOffset),
            Vec2(Widget::SIZE_AUTO, 0.7),
            Widget::Center
        );
        quitButton->clicked.connect<MainMenuScreen, &MainMenuScreen::onQuitClicked>(this);
        addWidget(quitButton);
    }

    if (hasDebug) {
        Button *debugButton = new Button(
            "Debug",
            &sharedAssets.defaultFont(),
            Vec2(unitConverter.horizontalCenter(), 1),
            Vec2(unitConverter.gameWidth() - 1.5f, 0.75),
            Color::black,
            Widget::Center
        );
        debugButton->clicked.connect<MainMenuScreen, &MainMenuScreen::onDebugClicked>(this);
        addWidget(debugButton);
    }
}

void MainMenuScreen::onPlayClicked(Button *sender) {
    sceneChangeRequested(LevelSelectScreen::sceneId, SceneInterface::Push);
}

void MainMenuScreen::onSettingsClicked(Button *sender) {
    sceneChangeRequested(SettingsScreen::sceneId, SceneInterface::Push);
}

void MainMenuScreen::onQuitClicked(Button *sender) {
    RailClimberPlatform::quit();
}

void MainMenuScreen::onDebugClicked(Button *sender) {
    sceneChangeRequested(DebugSettingsScreen::sceneId, SceneInterface::Push);
}
