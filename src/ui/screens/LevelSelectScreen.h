#pragma once

#include "ui/MenuScreen.h"
#include "ui/Button.h"

#define LEVELS_PER_SCREEN 15

class Label;
class StaticPicture;
class LevelMetadataRegistry;
class LevelPlayStatusRegistry;
class GameScene;

class LevelSelectScreen : public MenuScreen {
public:
    LevelSelectScreen(
        const UnitConverter &unitConverter,
        const SharedAssetRegistry &sharedAssets,
        const BitmapRenderer &bitmapRenderer,
        const LevelMetadataRegistry &metadataRegistry,
        const LevelPlayStatusRegistry &playStatusRegistry,
        GameScene *gameScene
    );

    void setupLevelButtons(int screenNumber);

    static constexpr const char* sceneId = "LevelSelect";

    struct TextureIds {
        static constexpr const char* button = "level-button";
        static constexpr const char* buttonDown = "level-button-down";
        static constexpr const char* buttonDisabled = "level-button-disabled";
        static constexpr const char* buttonPassed = "level-button-passed";
        static constexpr const char* buttonPassedDown = "level-button-passed-down";
    };

private:
    Button::TextureSet *mLevelButtonTextures;
    Button::TextureSet *mPassedLevelButtonTextures;

    Button *mLevelButton[LEVELS_PER_SCREEN];
    StaticPicture *mTinyMedal[LEVELS_PER_SCREEN];
    int mLevelScreen = 0;
    int mLevelScreenCount = 1;
    GameScene *mGameScene;
    const LevelMetadataRegistry &mMetadataRegistry;
    const LevelPlayStatusRegistry &mPlayStatusRegistry;
    Button *mNextButton;
    Button *mPreviousButton;

    void onBackClicked(Button *sender);
    void onNextScreenClicked(Button *sender);
    void onPreviousScreenClicked(Button *sender);
    void onLevelClicked(Button *sender);
};
