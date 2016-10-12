LOCAL_PATH:= $(call my-dir)

include $(LOCAL_PATH)/Box2D/Android.mk
include $(LOCAL_PATH)/expat/Android.mk
include $(LOCAL_PATH)/RailClimber-Android.mk

include $(CLEAR_VARS)

LOCAL_MODULE    := RailClimberNative
LOCAL_SRC_FILES := RailClimber/platform_specific/android/main_android.cpp
LOCAL_CFLAGS    := -Wall
LOCAL_LDLIBS    := -llog -lGLESv2 -ljnigraphics
LOCAL_SHARED_LIBRARIES := Box2D
LOCAL_STATIC_LIBRARIES := RailClimber expat

include $(BUILD_SHARED_LIBRARY)
