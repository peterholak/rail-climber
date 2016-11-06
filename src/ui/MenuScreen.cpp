#include "MenuScreen.h"
#include "Logging.h"

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

Vec2 MenuScreen::below(const Widget *other, float padding) {
    return other->bottomLeft() - Vec2{ 0, other->height() } - Vec2{ 0, padding };
}

Vec2 MenuScreen::above(const Widget *other, float padding) {
    return other->topLeft() + Vec2{ 0, padding };
}

Vec2 MenuScreen::matchHeight(const Widget *other) {
    return Vec2{ Widget::SIZE_AUTO, other->height() };
}

Vec2 MenuScreen::alignWithYx(const Widget *widgetY, const Widget *widgetX, const Vec2 &size) {
    if (size.x() == Widget::SIZE_AUTO || size.y() == Widget::SIZE_AUTO) {
        Log::warn("Relative coordinates don't work with auto-size yet.");
    }
    return Vec2{ widgetX->center().x() - size.x() / 2.0f, widgetY->center().y() - size.y() / 2.0f };
}

Vec2 MenuScreen::alignWithY(float x, const Widget *other, const Vec2 &size) {
    if (size.x() == Widget::SIZE_AUTO || size.y() == Widget::SIZE_AUTO) {
        Log::warn("Relative coordinates don't work with auto-size yet.");
    }
    return Vec2{ x, other->center().y() - size.y() / 2.0f };
}
