include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/expat
LOCAL_CFLAGS := -Wall -fexceptions

LOCAL_MODULE    := RailClimber
LOCAL_SRC_FILES := \
RailClimber/GameObject.cpp \
RailClimber/Rail.cpp \
RailClimber/StaticObject.cpp \
RailClimber/Vec2.cpp \
RailClimber/World.cpp \
RailClimber/Game.cpp \
RailClimber/Logging.cpp \
RailClimber/Sound.cpp \
RailClimber/WaveGenerator.cpp \
RailClimber/Enemy.cpp \
RailClimber/Settings.cpp \
RailClimber/Translation.cpp \
RailClimber/graphics/Painter.cpp \
RailClimber/graphics/Font.cpp \
RailClimber/graphics/Texture.cpp \
RailClimber/graphics/BufferObject.cpp \
RailClimber/graphics/ShaderProgram.cpp \
RailClimber/ui/Menu.cpp \
RailClimber/ui/Widget.cpp \
RailClimber/ui/Button.cpp \
RailClimber/ui/Label.cpp \
RailClimber/ui/AnimationWidget.cpp \
RailClimber/ui/tip_animations/AnimationLevel1.cpp \
RailClimber/ui/tip_animations/AnimationLevel3.cpp \
RailClimber/ui/tip_animations/AnimationLevel4.cpp \
RailClimber/ui/tip_animations/AnimationLevel7.cpp \
RailClimber/achievements/AchievementManager.cpp \
RailClimber/achievements/Achievement.cpp \
RailClimber/ForceField.cpp \
RailClimber/platform_specific/android/android_core.cpp \
RailClimber/platform_specific/android/android_sound.cpp \
RailClimber/platform_specific/android/android_graphics.cpp \
RailClimber/leveltool/LevelTool.cpp \
RailClimber/3rdparty/poly2tri/common/shapes.cpp \
RailClimber/3rdparty/poly2tri/sweep/advancing_front.cpp \
RailClimber/3rdparty/poly2tri/sweep/cdt.cpp \
RailClimber/3rdparty/poly2tri/sweep/sweep.cpp \
RailClimber/3rdparty/poly2tri/sweep/sweep_context.cpp \
RailClimber/3rdparty/clipper/clipper.cpp

include $(BUILD_STATIC_LIBRARY)