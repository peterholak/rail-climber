#include "ui/StaticPicture.h"
#include <SDL_events.h>
#include "GuiCase.h"
#include "ui/Label.h"
#include "ui/Button.h"

void GuiCase::initializeCase() {
    mLabel = new Label("text label at 0, 0", Vec2(0, 0), &mSharedAssets.defaultFont(), Color(255, 0, 0, 128));
    mStaticPicture = new StaticPicture("pictures/game/ball.png", Vec2(0, 0), Vec2(Widget::SIZE_AUTO, 2));
    mButton = new Button("Text button yo", &mSharedAssets.defaultFont(), Vec2(3, 3), Vec2(3, Widget::SIZE_AUTO));
    mButton->clicked.connect<GuiCase, &GuiCase::onButtonClicked>(this);
}

void GuiCase::drawCase(const BitmapRenderer &bitmapRenderer) {
    mLabel->draw(bitmapRenderer);
    mStaticPicture->draw(bitmapRenderer);
    mButton->draw(bitmapRenderer);
}

void GuiCase::handleCaseEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        Vec2 position = mUnitConverter.inGameXY({ event.button.x, event.button.y });
        mButton->handleTouchPress(position);
    }else if (event.type == SDL_MOUSEBUTTONUP) {
        Vec2 position = mUnitConverter.inGameXY({ event.button.x, event.button.y });
        mButton->handleTouchRelease(position);
    }
}

void GuiCase::onButtonClicked(Button *sender) {
    mLabel->setText("clicked!");
}
