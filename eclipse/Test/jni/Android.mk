LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := spotify
LOCAL_SRC_FILES := libspotify.so

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := Test
LOCAL_SRC_FILES := blah_Test.cpp
LOCAL_SHARED_LIBRARIES += libspotify
LOCAL_LDLIBS := -llog
LOCAL_LDLIBS += -lOpenSLES
LOCAL_CFLAGS := -g

include $(BUILD_SHARED_LIBRARY)