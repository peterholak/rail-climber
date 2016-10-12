#pragma once

#include "ui/MenuScreen.h"
#include "ui/Button.h"

class Label;
class StaticPicture;
class GameScene;
class UnitConverter;
class SharedAssetRegistry;

class FinishedMenu : public MenuScreen {
public:
    FinishedMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer, GameScene *gameScene);

    static constexpr const char* sceneId = "Finished";

private:
    Label *mTimeLabel;
    StaticPicture *mMedalPicture;
    Label *mNextMedalLabel;

    GameScene *mGameScene;

    void onRetryClicked(Button *sender);
    void onForwardClicked(Button *sender);
    void onBackClicked(Button *sender);
};
