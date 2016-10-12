#include "LevelSelectScreen.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/StaticPicture.h"
#include "game_logic/LevelMetadata.h"
#include <string>
#include "game_logic/LevelMetadataRegistry.h"
#include "game_logic/LevelPlayStatusRegistry.h"
#include "game_logic/GameScene.h"
#include "Logging.h"
#include "Translation.h"
#include <cassert>
#include <cmath>
#include "game_logic/UnitConverter.h"
#include "graphics/SharedAssetRegistry.h"

using namespace std;

LevelSelectScreen::LevelSelectScreen(
    const UnitConverter &unitConverter,
    const SharedAssetRegistry &sharedAssets,
    const BitmapRenderer &bitmapRenderer,
    const LevelMetadataRegistry &metadataRegistry,
    const LevelPlayStatusRegistry &playStatusRegistry,
    GameScene *gameScene
) : MenuScreen(sceneId, unitConverter, sharedAssets, bitmapRenderer), mMetadataRegistry(metadataRegistry), mPlayStatusRegistry(playStatusRegistry), mGameScene(gameScene)
{
    mFullScreen = true;

    unsigned int levelsPerScreen = LEVELS_PER_SCREEN;
    const Font *font = &sharedAssets.defaultFont();

    // TODO: do not show next screen button if this is less than levels per screen
    mLevelScreenCount = int(ceil((float)mMetadataRegistry.getLevelCount() / LEVELS_PER_SCREEN));

    mLevelButtonTextures = new Button::TextureSet(
        &sharedAssets.getUiTexture("level"),
        &sharedAssets.getUiTexture("level-down"),
        &sharedAssets.getUiTexture("level-disabled")
    );
    mPassedLevelButtonTextures = new Button::TextureSet(&sharedAssets.getUiTexture("level-passed"), &sharedAssets.getUiTexture("level-passed-down"));

    addWidget(new Label(_("Select a level"), Vec2(unitConverter.horizontalCenter(), 5.9f), font, Color::black, Widget::TopCenter, Vec2(4.0f, Widget::SIZE_AUTO)));
    Button *backButton = new Button(
        new Button::TextureSet(&sharedAssets.getUiTexture("back"), &sharedAssets.getUiTexture("back-down")),
        Vec2(unitConverter.gameWidth() - 0.8f, 0.9f),
        Vec2(0.8f, Widget::SIZE_AUTO)
    );
    backButton->clicked.connect<LevelSelectScreen, &LevelSelectScreen::onBackClicked>(this);
    addWidget(backButton);

    float buttonSize = 0.9f, gapSize = 0.15f;
    float x = (unitConverter.gameWidth() - (buttonSize * 5.0f + gapSize * 4.0f)) / 2.0f, y = 4.0f;

    for (unsigned int i = 0; i < levelsPerScreen; i++) {
        const LevelMetadata *level = mMetadataRegistry.getMetadataForLevel(i);
        auto &playStatus = mPlayStatusRegistry.getLevelPlayStatus(i);
        bool canPlay = mPlayStatusRegistry.canPlayLevel(level->number);
        char num[3];
        snprintf(num, 3, "%d", level->number + 1);
        mLevelButton[i] = new Button(
            num,
            font,
            Vec2(x, y),
            Vec2(buttonSize, buttonSize),
            Color::black,
            Widget::BottomCenter,
            playStatus.passed ? mPassedLevelButtonTextures : mLevelButtonTextures
        );
        mLevelButton[i]->setEnabled(canPlay);
        mLevelButton[i]->setUserData(level->number);
        mLevelButton[i]->clicked.connect<LevelSelectScreen, &LevelSelectScreen::onLevelClicked>(this);
        auto medal = mPlayStatusRegistry.getReachedMedal(i);
        mTinyMedal[i] = new StaticPicture(
            &sharedAssets.getUiTexture(string("tiny-medal-") + LevelPlayStatus::medalToString(medal)),
            mLevelButton[i]->topLeft() + Vec2(0.0f, -0.25f),
            Vec2(0.4f, Widget::SIZE_AUTO),
            Widget::Center
        );
        if (medal == LevelPlayStatus::NoMedal) {
            mTinyMedal[i]->setVisible(false);
        }
        addWidget(mLevelButton[i]);
        addWidget(mTinyMedal[i]);
        x += buttonSize + gapSize;
        if (x > unitConverter.gameWidth() * 0.8f) {
            x = (unitConverter.gameWidth() - (buttonSize * 5.0f + gapSize * 4.0f)) / 2.0f;
            y -= 1.2f;
        }
    }

    y = 0.9f;
    x = (unitConverter.gameWidth() - (buttonSize * 5.0f + gapSize * 4.0f)) / 2.0f + buttonSize / 2.0f;
    mNextButton = new Button(
        new Button::OwnedTextureSet("pictures/ui/levels-next.png", "pictures/ui/levels-next.png"),
        Vec2(unitConverter.gameWidth() / 2.0f, y),
        Vec2(Widget::SIZE_AUTO, 0.6f)
    );
    mNextButton->setVisible(levelsPerScreen <= mMetadataRegistry.getLevelCount());
    mNextButton->clicked.connect<LevelSelectScreen, &LevelSelectScreen::onNextScreenClicked>(this);
    addWidget(mNextButton);

    mPreviousButton = new Button(
        new Button::OwnedTextureSet("pictures/ui/levels-prev.png", "pictures/ui/levels-prev.png"),
        Vec2(x, y),
        Vec2(Widget::SIZE_AUTO, 0.6f)
    );
    mPreviousButton->setVisible(false);
    mPreviousButton->clicked.connect<LevelSelectScreen, &LevelSelectScreen::onPreviousScreenClicked>(this);
    addWidget(mPreviousButton);
}

void LevelSelectScreen::onBackClicked(Button *sender) {
    finished();
}

void LevelSelectScreen::onNextScreenClicked(Button *sender) {
    setupLevelButtons(mLevelScreen + 1);
}

void LevelSelectScreen::onPreviousScreenClicked(Button *sender) {
    setupLevelButtons(mLevelScreen - 1);
}

void LevelSelectScreen::onLevelClicked(Button *sender) {
    int levelsPerScreen = LEVELS_PER_SCREEN;
    int levelNum = sender->getIntUserData();
    levelNum += mLevelScreen * levelsPerScreen;
    mGameScene->playLevel(levelNum);
    sceneChangeRequested(mGameScene->getSceneId(), SceneInterface::Push);
}

void LevelSelectScreen::setupLevelButtons(int screenNumber) {
    assert(screenNumber >= 0 && screenNumber < mLevelScreenCount);

    unsigned int levelsPerScreen = LEVELS_PER_SCREEN;

    mLevelScreen = screenNumber;

    mPreviousButton->setVisible(mLevelScreen > 0);
    mNextButton->setVisible(mLevelScreen < mLevelScreenCount - 1);

    for (unsigned int i = 0; i < levelsPerScreen; i++) {
        if (i + levelsPerScreen * mLevelScreen >= mMetadataRegistry.getLevelCount()) {
            mLevelButton[i]->setVisible(false);
            mTinyMedal[i]->setVisible(false);
            continue;
        }
        const LevelMetadata *level = mMetadataRegistry.getMetadataForLevel(i + levelsPerScreen * mLevelScreen);
        auto &playStatus = mPlayStatusRegistry.getLevelPlayStatus(level->number);
        bool canPlay = mPlayStatusRegistry.canPlayLevel(level->number);
        char num[3];
        snprintf(num, 3, "%d", level->number + 1);

        mLevelButton[i]->setEnabled(canPlay);
        mLevelButton[i]->setVisible(true);
        mLevelButton[i]->setText(num);
        mLevelButton[i]->setImageTextures(playStatus.passed ? mPassedLevelButtonTextures : mLevelButtonTextures);
        auto medal = mPlayStatusRegistry.getReachedMedal(level->number);
        if (medal != LevelPlayStatus::NoMedal) {
            mTinyMedal[i]->setVisible(true);
            mTinyMedal[i]->changePicture(&mSharedAssets.getUiTexture(string("tiny-medal-") + LevelPlayStatus::medalToString(medal)));
        } else {
            mTinyMedal[i]->setVisible(false);
        }
    }
}
