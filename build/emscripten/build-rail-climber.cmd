em++ ^
-O0 ^
-g4 ^
--preload-file ../../data ^
-Ic:\src\emlibs\expat\lib ^
-Ic:\src\emlibs\box2d ^
-Ic:\src\emlibs\glm ^
-Ic:\src\emlibs\SDL_ttf ^
-I../../src ^
-DPLATFORM_EMSCRIPTEN ^
-std=c++11 ^
-s USE_SDL=2 ^
-s USE_SDL_IMAGE=2 ^
-o ..\..\bin\RailClimber.html ^
c:\src\emlibs\Box2D.bc ^
c:\src\emlibs\expat.bc ^
c:\src\emlibs\SDL_ttf.bc ^
c:\src\emlibs\libfreetype.a ^
../../src/3rdparty/clipper/clipper.cpp ^
../../src/3rdparty/poly2tri/common/shapes.cpp ^
../../src/3rdparty/poly2tri/sweep/advancing_front.cpp ^
../../src/3rdparty/poly2tri/sweep/cdt.cpp ^
../../src/3rdparty/poly2tri/sweep/sweep.cpp ^
../../src/3rdparty/poly2tri/sweep/sweep_context.cpp ^
../../src/ExpatParser.cpp ^
../../src/game_logic/CollisionFilter.cpp ^
../../src/game_logic/Enemy.cpp ^
../../src/game_logic/GameScene.cpp ^
../../src/game_logic/LevelMetadataRegistry.cpp ^
../../src/game_logic/LevelPlayStatusRegistry.cpp ^
../../src/game_logic/PolygonalObject.cpp ^
../../src/game_logic/Rail.cpp ^
../../src/game_logic/SceneManager.cpp ^
../../src/game_logic/StaticObject.cpp ^
../../src/game_logic/UnitConverter.cpp ^
../../src/game_logic/Vec2.cpp ^
../../src/game_logic/Vibrator.cpp ^
../../src/game_logic/WaveGenerator.cpp ^
../../src/game_logic/World.cpp ^
../../src/graphics/BitmapRenderer.cpp ^
../../src/graphics/BufferObject.cpp ^
../../src/graphics/Font.cpp ^
../../src/graphics/Painter.cpp ^
../../src/graphics/RenderDriver.cpp ^
../../src/graphics/renderers/EnemyRenderer.cpp ^
../../src/graphics/renderers/GrassRenderer.cpp ^
../../src/graphics/renderers/PolygonalObjectRenderer.cpp ^
../../src/graphics/renderers/RailRenderer.cpp ^
../../src/graphics/renderers/SmokeRenderer.cpp ^
../../src/graphics/renderers/WallRenderer.cpp ^
../../src/graphics/renderers/WaveGeneratorRenderer.cpp ^
../../src/graphics/renderers/WorldRenderer.cpp ^
../../src/graphics/ShaderProgram.cpp ^
../../src/graphics/SharedAssetRegistry.cpp ^
../../src/graphics/Texture.cpp ^
../../src/graphics/Theme.cpp ^
../../src/level_loader/svg/game_objects/EnemyCreator.cpp ^
../../src/level_loader/svg/game_objects/GrassCreator.cpp ^
../../src/level_loader/svg/game_objects/PlayerStartPosition.cpp ^
../../src/level_loader/svg/game_objects/RailCreator.cpp ^
../../src/level_loader/svg/game_objects/StaticObjectCreator.cpp ^
../../src/level_loader/svg/game_objects/WaveGeneratorCreator.cpp ^
../../src/level_loader/svg/InkscapeSvgLevelLoader.cpp ^
../../src/level_loader/XmlMetadataLoader.cpp ^
../../src/Logging.cpp ^
../../src/platform_specific/emscripten/emscripten_core.cpp ^
../../src/platform_specific/emscripten/emscripten_graphics.cpp ^
../../src/platform_specific/emscripten/emscripten_sound.cpp ^
../../src/platform_specific/emscripten/main_emscripten.cpp ^
../../src/platform_specific/emscripten/RailClimberAppEm.cpp ^
../../src/Settings.cpp ^
../../src/Sound.cpp ^
../../src/Translation.cpp ^
../../src/ui/AnimationWidget.cpp ^
../../src/ui/Button.cpp ^
../../src/ui/GameMenu.cpp ^
../../src/ui/Label.cpp ^
../../src/ui/LevelSelectScreen.cpp ^
../../src/ui/MenuScreen.cpp ^
../../src/ui/screens/DeadMenu.cpp ^
../../src/ui/screens/DebugSettingsScreen.cpp ^
../../src/ui/screens/FinishedMenu.cpp ^
../../src/ui/screens/GameTipMenu.cpp ^
../../src/ui/screens/LevelSelectScreen.cpp ^
../../src/ui/screens/MainMenuScreen.cpp ^
../../src/ui/screens/MoreLevelsMenu.cpp ^
../../src/ui/screens/PausedMenu.cpp ^
../../src/ui/screens/SettingsScreen.cpp ^
../../src/ui/tip_animations/AnimationLevel1.cpp ^
../../src/ui/tip_animations/AnimationLevel3.cpp ^
../../src/ui/tip_animations/AnimationLevel4.cpp ^
../../src/ui/tip_animations/AnimationLevel7.cpp ^
../../src/ui/Widget.cpp