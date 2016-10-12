#include "MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen(
    const char *sceneId,
    const UnitConverter &unitConverter,
    const SharedAssetRegistry &sharedAssets,
    const BitmapRenderer &bitmapRenderer
) : mSceneId(sceneId), mUnitConverter(unitConverter), mSharedAssets(sharedAssets), mBitmapRenderer(bitmapRenderer) {
}

MenuScreen::~MenuScreen() {
    for (auto widget : mWidgets) {
        // TODO: make it clearer that menu screen owns the widgets and they get auto-deleted so that subclasses shouldn't do that manually??
        delete widget;
    }
}

bool MenuScreen::handleTouchPress(const Vec2& inGameXY) {
    for (auto widget : mWidgets) {
        if (!widget->isVisible()) {
            continue;
        }

        if (
            widget->wantsEventsOutsideOfBounds() || (
                inGameXY.x() >= widget->bottomLeft().x() && inGameXY.y() >= widget->bottomLeft().y() &&
                inGameXY.x() <= widget->bottomLeft().x() + widget->width() && inGameXY.y() <= widget->topLeft().y()
            )
        ) {
            widget->handleTouchPress(inGameXY);
        }
    }
    return true;
}

bool MenuScreen::handleTouchMove(const Vec2& inGameXY) {
    for (auto widget : mWidgets) {
        widget->handleTouchMove(inGameXY);
    }
    return true;
}

bool MenuScreen::handleTouchRelease(const Vec2& inGameXY) {
    for (auto widget : mWidgets) {
        widget->handleTouchRelease(inGameXY);
    }
    return true;
}

void MenuScreen::addWidget(Widget *widget) {
    mWidgets.push_back(widget);
}

void MenuScreen::reload() {
    for (auto widget : mWidgets) {
        widget->reload();
    }
}

void MenuScreen::unload() {
    for (auto widget : mWidgets) {
        widget->unload();
    }
}

void MenuScreen::tick() {
    for (auto widget : mWidgets) {
        widget->tick();
    }
}

void MenuScreen::manualTick() {
    for (auto widget : mWidgets) {
        widget->tick(true);
    }
}

void MenuScreen::render() {
    if (isFullScreen()) {
        mBitmapRenderer.renderTexture(mSharedAssets.getUiTexture("background"), 0, 0, mUnitConverter.gameWidth(), mUnitConverter.gameHeight());
    }else{
        mBitmapRenderer.renderTexture(mSharedAssets.getUiTexture("overlay"), 0.5f, 2.0f, mUnitConverter.gameWidth() - 1.0f, 6.0f);
    }

    for (auto widget : mWidgets) {
        if (widget->isVisible()) {
            widget->draw(mBitmapRenderer);
        }
    }
}
