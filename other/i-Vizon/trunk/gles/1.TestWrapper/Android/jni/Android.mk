LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := testWrapper
LOCAL_SRC_FILES := TestWrapper.cpp

include $(BUILD_SHARED_LIBRARY)
