include $(CLEAR_VARS)

LOCAL_MODULE    := Box2D
LOCAL_C_INCLUDES += $(LOCAL_PATH)
FILE_LIST := $(wildcard $(LOCAL_PATH)/Box2D/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/Box2D/*/*/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_SHARED_LIBRARY)