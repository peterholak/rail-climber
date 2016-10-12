#pragma once

#include "ui/MenuScreen.h"

class Painter;
class Button;
class GameScene;

class PausedMenu : public MenuScreen {
public:
    PausedMenu(const UnitConverter &unitConverter, const SharedAssetRegistry &sharedAssets, const BitmapRenderer &bitmapRenderer, GameScene *gameScene);
    virtual bool isTransparent() const { return true; }

    static constexpr const char* sceneId = "Paused";

private:
    Button *mSoundButton;
    GameScene *mGameScene;

    void onSoundToggled(Button *sender);
    void onResumeClicked(Button *sender);
    void onRestartClicked(Button *sender);
    void onBackToMenuClicked(Button *sender);
};
