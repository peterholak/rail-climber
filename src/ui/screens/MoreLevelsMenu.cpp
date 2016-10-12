#include "MoreLevelsMenu.h"
#include "MainMenuScreen.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "Translation.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

MoreLevelsMenu::MoreLevelsMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer)
{
    const Font *font = &sharedAssets.defaultFont();

    addWidget(new Label(_("Congratulations!"), Vec2(unitConverter.horizontalCenter(), 6.8f), font, Color::black, Widget::BottomCenter, Vec2(
        unitConverter.gameWidth() - 2.0f, 0.0f)));
    addWidget(new Label(_("More levels"), Vec2(unitConverter.horizontalCenter(), 5.1f), font, Color::black, Widget::BottomCenter, Vec2(Widget::SIZE_AUTO, 0.6f)));
    addWidget(new Label(_("coming soon"), Vec2(unitConverter.horizontalCenter(), 4.5f), font, Color::black, Widget::BottomCenter, Vec2(Widget::SIZE_AUTO, 0.6f)));

    Button *backButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")),
        Vec2(1.1f, 2.75f),
        Vec2(0.8f, Widget::SIZE_AUTO)
    );
    backButton->clicked.connect<MoreLevelsMenu, &MoreLevelsMenu::onBackClicked>(this);
    addWidget(backButton);
}

void MoreLevelsMenu::onBackClicked(Button *sender) {
    sceneChangeRequested(MainMenuScreen::sceneId, SceneInterface::Replacement);
}
