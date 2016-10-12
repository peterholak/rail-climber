#include "GameTipMenu.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/StaticPicture.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

GameTipMenu::GameTipMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer)
    : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer)
{
    const Font *font = &sharedAssets.defaultFont();
    const Font *smallFont = &sharedAssets.defaultFont(); //TODO

    addWidget(new Button("Ok", font, Vec2(unitConverter.horizontalCenter() - 1.5f, 2.65f), Vec2(3.0f, 0.6f)));
    mTipLabel = new Label(
        " ",
        Vec2(unitConverter.horizontalCenter(), 7.5f),
        smallFont,
        Color::black,
        Widget::TopCenter,
        Vec2(unitConverter.gameWidth() - 1.5f, Widget::SIZE_AUTO)
    );
    mTipPicture = new StaticPicture(&sharedAssets.getUiTexture("back"), Vec2(unitConverter.horizontalCenter(), 5.4), Vec2(Widget::SIZE_AUTO, 3.5), Widget::Center);
    addWidget(mTipLabel);
    addWidget(mTipPicture);
}
