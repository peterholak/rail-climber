#include "DeadMenu.h"
#include "MainMenuScreen.h"
#include "ui/Label.h"
#include "ui/Button.h"
#include "game_logic/GameScene.h"
#include "Translation.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

DeadMenu::DeadMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer, GameScene *gameScene)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer), mGameScene(gameScene)
{
    const Font *font = &sharedAssets.defaultFont();

    addWidget(new Label(_("You died"), Vec2(unitConverter.horizontalCenter(), 7.5f), font, Color::black, Widget::TopCenter, Vec2(Widget::SIZE_AUTO, 1.0f)));

    Button *retryButton = new Button(new Button::OwnedTextureSet("pictures/ui/retry.png"), Vec2(unitConverter.horizontalCenter(), 4.7f), Vec2(1.5f, Widget::SIZE_AUTO));
    retryButton->clicked.connect<DeadMenu, &DeadMenu::onRetryClicked>(this);
    addWidget(retryButton);

    Button *backButton = new Button(new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")), Vec2(1.1f, 2.75f), Vec2(0.8f, Widget::SIZE_AUTO));
    backButton->clicked.connect<DeadMenu, &DeadMenu::onBackClicked>(this);
    addWidget(backButton);
}

void DeadMenu::onRetryClicked(Button *sender) {
    mGameScene->restartLevel();
}

void DeadMenu::onBackClicked(Button *sender) {
    sceneChangeRequested(MainMenuScreen::sceneId, SceneInterface::Replacement);
}
