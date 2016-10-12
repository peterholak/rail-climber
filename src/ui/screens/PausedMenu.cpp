#include "Settings.h"
#include "PausedMenu.h"
#include "MainMenuScreen.h"
#include "ui/Button.h"
#include "game_logic/GameScene.h"
#include "Translation.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

PausedMenu::PausedMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer, GameScene *gameScene)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer), mGameScene(gameScene)
{
    const Font *font = &sharedAssets.defaultFont();

    Button *resumeButton = new Button(_("Resume"), font, Vec2(0.75f, 6.29f), Vec2(unitConverter.gameWidth() - 1.5f, 1.0f));
    resumeButton->clicked.connect<PausedMenu, &PausedMenu::onResumeClicked>(this);
    addWidget(resumeButton);

    mSoundButton = new Button(_("Sound off"), font, Vec2(0.75f, 5.11f), Vec2(unitConverter.gameWidth() - 1.5f, 1.0f));
    mSoundButton->clicked.connect<PausedMenu, &PausedMenu::onSoundToggled>(this);
    addWidget(mSoundButton);

    Button *restartButton = new Button(_("Restart level"), font, Vec2(0.75f, 3.88f), Vec2(unitConverter.gameWidth() - 1.5f, 1.0f));
    restartButton->clicked.connect<PausedMenu, &PausedMenu::onRestartClicked>(this);
    addWidget(restartButton);

    Button *backButton = new Button(_("Main menu"), font, Vec2(0.75f, 2.63f), Vec2(unitConverter.gameWidth() - 1.5f, 1.0f));
    backButton->clicked.connect<PausedMenu, &PausedMenu::onBackToMenuClicked>(this);
    addWidget(backButton);
    if (Settings::getSavedBool("settings", "sound", true) == false) {
        mSoundButton->setText(_("Sound on"));
    }
}

void PausedMenu::onResumeClicked(Button *sender) {
    finished();
}

void PausedMenu::onRestartClicked(Button *sender) {
    mGameScene->restartLevel();
    finished();
}

void PausedMenu::onBackToMenuClicked(Button *sender) {
    sceneChangeRequested(MainMenuScreen::sceneId, SceneInterface::Replacement);
}

void PausedMenu::onSoundToggled(Button *sender) {
    // TODO
}
