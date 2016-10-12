cmake_minimum_required(VERSION 2.8)
project (RailClimber)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-reorder -Wno-comment -Wno-deprecated-declarations")
add_definitions(-DPLATFORM_SDL -U__STRICT_ANSI__)

find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
find_package (SDL2_ttf REQUIRED)
find_package (SDL2_mixer REQUIRED)
find_package (yaml-cpp REQUIRED)
find_package (EXPAT REQUIRED)
find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package (GLM REQUIRED)
find_path(BOX2D_INCLUDE_DIRS Box2D HINTS ENV MINGDIR PATH_SUFFIXES include/Box2D include)
find_library(BOX2D_LIBRARIES NAMES Box2D.dll Box2D)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
set(SRC_PLATFORM ${SRC}/platform_specific)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SRC}/../bin)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -iquote ${SRC}")

add_executable(RailClimber
${SRC}/3rdparty/poly2tri/common/shapes.cpp
${SRC}/3rdparty/poly2tri/sweep/advancing_front.cpp
${SRC}/3rdparty/poly2tri/sweep/cdt.cpp
${SRC}/3rdparty/poly2tri/sweep/sweep.cpp
${SRC}/3rdparty/poly2tri/sweep/sweep_context.cpp
${SRC}/3rdparty/clipper/clipper.cpp
${SRC}/game_logic/CollisionFilter.cpp
${SRC}/game_logic/Enemy.cpp
${SRC}/game_logic/GameScene.cpp
${SRC}/game_logic/PolygonalObject.cpp
${SRC}/game_logic/Rail.cpp
${SRC}/game_logic/StaticObject.cpp
${SRC}/game_logic/UnitConverter.cpp
${SRC}/game_logic/Vec2.cpp
${SRC}/game_logic/WaveGenerator.cpp
${SRC}/game_logic/World.cpp
${SRC}/graphics/Font.cpp
${SRC}/graphics/BufferObject.cpp
${SRC}/graphics/BitmapRenderer.cpp
${SRC}/graphics/Texture.cpp
${SRC}/graphics/Theme.cpp
${SRC}/graphics/ShaderProgram.cpp
${SRC}/graphics/renderers/RailRenderer.cpp
${SRC}/graphics/renderers/WaveGeneratorRenderer.cpp
${SRC}/graphics/renderers/PolygonalObjectRenderer.cpp
${SRC}/graphics/renderers/SmokeRenderer.cpp
${SRC}/graphics/renderers/WallRenderer.cpp
${SRC}/level_loader/XmlMetadataLoader.cpp
${SRC}/level_loader/svg/InkscapeSvgLevelLoader.cpp
${SRC}/level_loader/svg/game_objects/EnemyCreator.cpp
${SRC}/level_loader/svg/game_objects/GrassCreator.cpp
${SRC}/level_loader/svg/game_objects/PlayerStartPosition.cpp
${SRC}/level_loader/svg/game_objects/RailCreator.cpp
${SRC}/level_loader/svg/game_objects/StaticObjectCreator.cpp
${SRC}/level_loader/svg/game_objects/WaveGeneratorCreator.cpp
${SRC}/ExpatParser.cpp
${SRC}/Logging.cpp
${SRC}/Settings.cpp
${SRC}/Sound.cpp
${SRC}/Translation.cpp
${SRC}/ui/AnimationWidget.cpp
${SRC}/ui/Button.cpp
${SRC}/ui/Label.cpp
${SRC}/ui/MenuScreen.cpp
${SRC}/ui/Widget.cpp
${SRC}/ui/tip_animations/AnimationLevel1.cpp
${SRC}/ui/tip_animations/AnimationLevel3.cpp
${SRC}/ui/tip_animations/AnimationLevel4.cpp
${SRC}/ui/tip_animations/AnimationLevel7.cpp
${SRC_PLATFORM}/sdl/main_sdl.cpp
${SRC_PLATFORM}/sdl/sdl_core.cpp
${SRC_PLATFORM}/sdl/sdl_sound.cpp
${SRC_PLATFORM}/sdl/sdl_graphics.cpp
${SRC_PLATFORM}/sdl/RailClimberApp.cpp
${SRC}/game_logic/Vibrator.cpp
${SRC}/game_logic/Vibrator.h
${SRC}/level_loader/LevelLoaderInterface.h
${SRC}/game_logic/LevelMetadataRegistry.cpp
${SRC}/game_logic/LevelMetadataRegistry.h
${SRC}/level_loader/LevelMetadataLoaderInterface.h
${SRC}/game_logic/LevelPlayStatus.h
${SRC}/game_logic/LevelPlayStatusRegistry.cpp
${SRC}/game_logic/LevelPlayStatusRegistry.h
${SRC}/game_logic/SceneManager.cpp
${SRC}/game_logic/SceneManager.h
${SRC}/game_logic/SceneInterface.h
${SRC}/game_logic/Renderable.h
${SRC}/game_logic/RenderDriverInterface.h
${SRC}/graphics/RenderDriver.cpp
${SRC}/graphics/RenderDriver.h
${SRC}/ui/screens/MainMenuScreen.cpp
${SRC}/ui/screens/MainMenuScreen.h
${SRC}/ui/screens/LevelSelectScreen.cpp
${SRC}/ui/screens/LevelSelectScreen.h
${SRC}/ui/screens/SettingsScreen.cpp
${SRC}/ui/screens/SettingsScreen.h
${SRC}/ui/screens/DebugSettingsScreen.cpp
${SRC}/ui/screens/DebugSettingsScreen.h
${SRC}/ui/screens/PausedMenu.cpp
${SRC}/ui/screens/PausedMenu.h
${SRC}/ui/screens/DeadMenu.cpp
${SRC}/ui/screens/DeadMenu.h
${SRC}/ui/screens/FinishedMenu.cpp
${SRC}/ui/screens/FinishedMenu.h
${SRC}/ui/screens/MoreLevelsMenu.cpp
${SRC}/ui/screens/MoreLevelsMenu.h
${SRC}/ui/screens/GameTipMenu.cpp
${SRC}/ui/screens/GameTipMenu.h
../../src/ui/GameMenu.cpp ../../src/ui/GameMenu.h ../../src/graphics/renderers/WorldRenderer.cpp ../../src/graphics/renderers/WorldRenderer.h ../../src/graphics/renderers/EnemyRenderer.cpp ../../src/graphics/renderers/EnemyRenderer.h ../../src/graphics/SharedAssetRegistry.cpp ../../src/graphics/SharedAssetRegistry.h ../../src/level_loader/YamlMetadataLoader.cpp ../../src/level_loader/YamlMetadataLoader.h ../../src/ui/StaticPicture.cpp ../../src/ui/StaticPicture.h ../../src/platform_specific/sdl/dev_sandbox/RailClimberSandbox.cpp ../../src/platform_specific/sdl/dev_sandbox/RailClimberSandbox.h ../../src/platform_specific/sdl/dev_sandbox/SandboxCase.h ../../src/platform_specific/sdl/dev_sandbox/GuiCase.cpp ../../src/platform_specific/sdl/dev_sandbox/GuiCase.h ../../src/graphics/Color.h ../../src/graphics/Color.cpp ../../src/ui/InvalidCoordinatesException.h ../../src/platform_specific/sdl/dev_sandbox/YamlLevelCase.cpp ../../src/platform_specific/sdl/dev_sandbox/YamlLevelCase.h)

include_directories (
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
	${SDL2_MIXER_INCLUDE_DIRS}
	${YAML_CPP_INCLUDE_DIR}
	${EXPAT_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${BOX2D_INCLUDE_DIRS}
)

target_link_libraries(RailClimber
	${SDLMAIN2_LIBRARY}
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${YAML_CPP_LIBRARIES}
	${EXPAT_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${BOX2D_LIBRARIES}
	${MINGW32_LIBRARIES}
)
