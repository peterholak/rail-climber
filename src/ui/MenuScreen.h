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
    MenuScreen(const char *sceneId, const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer);
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

    void addWidget(Widget *widget);

    bool isFullScreen() const { return mFullScreen; }
    const std::list<Widget*> getWidgets() const { return mWidgets; }

protected:
    const BitmapRenderer &mBitmapRenderer;
    const SharedAssetRegistry &mSharedAssets;
    const UnitConverter &mUnitConverter;
    bool mFullScreen = false;
    const char *mSceneId;
    std::list<Widget*> mWidgets;
};
