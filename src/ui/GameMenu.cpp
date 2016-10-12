#include "GameMenu.h"
#include "game_logic/SceneManager.h"

GameMenu::GameMenu(
    const UnitConverter &unitConverter,
    const SharedAssetRegistry &sharedAssets,
    const BitmapRenderer &bitmapRenderer,
    const LevelMetadataRegistry &metadataRegistry,
    const LevelPlayStatusRegistry &playStatusRegistry,
    GameScene *gameScene
) {
    mainMenu = new MainMenuScreen(unitConverter, sharedAssets, bitmapRenderer);
    settings = new SettingsScreen(unitConverter, sharedAssets, bitmapRenderer);
    debugSettings = new DebugSettingsScreen(unitConverter, sharedAssets, bitmapRenderer);
    levelSelect = new LevelSelectScreen(unitConverter, sharedAssets, bitmapRenderer, metadataRegistry, playStatusRegistry, gameScene);
    gameTipMenu = new GameTipMenu(unitConverter, sharedAssets, bitmapRenderer);
    pausedMenu = new PausedMenu(unitConverter, sharedAssets, bitmapRenderer, gameScene);
    deadMenu = new DeadMenu(unitConverter, sharedAssets, bitmapRenderer, gameScene);
    finishedMenu = new FinishedMenu(unitConverter, sharedAssets, bitmapRenderer, gameScene);
    moreLevelsMenu = new MoreLevelsMenu(unitConverter, sharedAssets, bitmapRenderer);
}

void GameMenu::registerWithSceneManager(SceneManager *sceneManager) {
    sceneManager->registerScene(mainMenu);
    sceneManager->registerScene(settings);
    sceneManager->registerScene(levelSelect);
    sceneManager->registerScene(debugSettings);
    sceneManager->registerScene(gameTipMenu);
    sceneManager->registerScene(pausedMenu);
    sceneManager->registerScene(deadMenu);
    sceneManager->registerScene(finishedMenu);
    sceneManager->registerScene(moreLevelsMenu);
}
