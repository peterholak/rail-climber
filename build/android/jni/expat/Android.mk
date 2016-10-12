include $(CLEAR_VARS)

LOCAL_MODULE    := expat
LOCAL_C_INCLUDES += $(LOCAL_PATH)/expat
LOCAL_CFLAGS += -DHAVE_EXPAT_CONFIG_H
LOCAL_SRC_FILES := expat/xmlparse.c expat/xmlrole.c expat/xmltok.c expat/xmltok_impl.c expat/xmltok_ns.c

include $(BUILD_STATIC_LIBRARY)