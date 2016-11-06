#pragma once

#include "ui/Widget.h"
#include <list>
#include <string>
#include <map>
#include "game_logic/SceneInterface.h"
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

class MenuScreen : public SceneInterface {
public:
    MenuScreen(
        const char *sceneId,
        const UnitConverter &unitConverter,
        const SharedAssetRegistry &sharedAssets,
        const BitmapRenderer &bitmapRenderer
    );
    virtual ~MenuScreen();
    void reload();
    void unload();
    void tick();
    void manualTick();

    virtual bool handleTouchPress(const Vec2& inGameXY);
    virtual bool handleTouchMove(const Vec2& inGameXY);
    virtual bool handleTouchRelease(const Vec2& inGameXY);

    virtual void render();
    virtual const char* getSceneId() const { return mSceneId; }

    template<class T>
    T* addWidget(T *widget) {
        mWidgets.push_back(widget);
        return widget;
    }

    bool isFullScreen() const { return mFullScreen; }
    const std::list<Widget*> getWidgets() const { return mWidgets; }

    static constexpr float DefaultPadding = 0.2f;

    static Vec2 below(const Widget* other, float padding = DefaultPadding);
    static Vec2 above(const Widget* other, float padding = DefaultPadding);
    static Vec2 matchHeight(const Widget* other);
    static Vec2 alignWithYx(const Widget *widgetY, const Widget *widgetX, const Vec2 &size);
    static Vec2 alignWithY(float x, const Widget* other, const Vec2& size);

protected:
    const BitmapRenderer &mBitmapRenderer;
    const SharedAssetRegistry &mSharedAssets;
    const UnitConverter &mUnitConverter;
    bool mFullScreen = false;
    const char *mSceneId;
    std::list<Widget*> mWidgets;
};
