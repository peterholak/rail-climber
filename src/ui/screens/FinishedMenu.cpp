#include "FinishedMenu.h"
#include "MainMenuScreen.h"
#include "game_logic/GameScene.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/StaticPicture.h"
#include "Translation.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

FinishedMenu::FinishedMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer, GameScene *gameScene)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer), mGameScene(gameScene)
{
    const Font *font = &sharedAssets.defaultFont();

    addWidget(new Label(_("Finished!"), Vec2(unitConverter.horizontalCenter(), 7.75f), font, Color::black, Widget::TopCenter, Vec2(Widget::SIZE_AUTO, 0.75f)));
    mTimeLabel = new Label("0:00.00", Vec2(unitConverter.horizontalCenter(), 6.5f), font, Color::black, Widget::Center, Vec2(Widget::SIZE_AUTO, 0.8f));
    mNextMedalLabel = new Label("High score: 0:00.00", Vec2(unitConverter.horizontalCenter(), 4.0f), font, Color::black, Widget::Center, Vec2(Widget::SIZE_AUTO, 0.5f));
    mMedalPicture = new StaticPicture(&sharedAssets.getUiTexture("medal-gold"), Vec2(unitConverter.horizontalCenter(), 5.15f), Vec2(1.5f, Widget::SIZE_AUTO), Widget::Center);
    addWidget(mTimeLabel);
    addWidget(mNextMedalLabel);
    addWidget(mMedalPicture);

    Button *retryButton = new Button(
        new Button::OwnedTextureSet("pictures/ui/retry.png", "pictures/ui/retry.png"),
        Vec2(unitConverter.horizontalCenter() - 0.05f, 2.75f),
        Vec2(0.7f, Widget::SIZE_AUTO)
    );
    retryButton->clicked.connect<FinishedMenu, &FinishedMenu::onRetryClicked>(this);
    addWidget(retryButton);

    Button *forwardButton = new Button(
        new Button::OwnedTextureSet("pictures/ui/forward.png", "pictures/ui/forward.png"),
        Vec2(unitConverter.gameWidth() - 1.3f, 2.75f),
        Vec2(0.9f, Widget::SIZE_AUTO)
    );
    forwardButton->clicked.connect<FinishedMenu, &FinishedMenu::onForwardClicked>(this);
    addWidget(forwardButton);

    Button *backButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")),
        Vec2(1.1f, 2.75f),
        Vec2(0.8f, Widget::SIZE_AUTO)
    );
    backButton->clicked.connect<FinishedMenu, &FinishedMenu::onBackClicked>(this);
    addWidget(backButton);
}

void FinishedMenu::onBackClicked(Button *sender) {
    sceneChangeRequested(MainMenuScreen::sceneId, SceneInterface::Replacement);
}

void FinishedMenu::onForwardClicked(Button *sender) {
    mGameScene->playNextLevel();
    finished();
}

void FinishedMenu::onRetryClicked(Button *sender) {
    mGameScene->restartLevel();
    finished();
}
