#pragma once

#include <vector>
#include "ui/screens/MainMenuScreen.h"
#include "ui/screens/SettingsScreen.h"
#include "ui/screens/DebugSettingsScreen.h"
#include "ui/screens/LevelSelectScreen.h"
#include "ui/screens/GameTipMenu.h"
#include "ui/screens/PausedMenu.h"
#include "ui/screens/DeadMenu.h"
#include "ui/screens/FinishedMenu.h"
#include "ui/screens/MoreLevelsMenu.h"

class SceneManager;
class GameScene;
class BitmapRenderer;
class SharedAssetRegistry;

class GameMenu {
public:
    GameMenu(
        const UnitConverter &unitConverter,
        const SharedAssetRegistry &sharedAssets,
        const BitmapRenderer &bitmapRenderer,
        const LevelMetadataRegistry &metadataRegistry,
        const LevelPlayStatusRegistry &playStatusRegistry,
        GameScene *gameScene
    );
    void registerWithSceneManager(SceneManager *sceneManager);

    SettingsScreen *settings;
    PausedMenu *pausedMenu;
    MoreLevelsMenu *moreLevelsMenu;
    MainMenuScreen *mainMenu;
    LevelSelectScreen *levelSelect;
    GameTipMenu *gameTipMenu;
    FinishedMenu *finishedMenu;
    DebugSettingsScreen *debugSettings;
    DeadMenu *deadMenu;
};


