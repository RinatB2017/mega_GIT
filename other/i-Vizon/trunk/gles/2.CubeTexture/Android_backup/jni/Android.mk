LOCAL_PATH := $(call my-dir)

IVIZON_TRUNK ?= /opt/i-vizon/trunk
GLES = $(IVIZON_TRUNK)/gles
GLES_WRAPPER = $(GLES)/Wrapper/
UTILS = $(IVIZON_TRUNK)/utils
COMMON = $(IVIZON_TRUNK)/common

include $(CLEAR_VARS)

LOCAL_MODULE    := CubeTest

LOCAL_C_INCLUDES += $(GLES_WRAPPER)/inc/
LOCAL_C_INCLUDES := $(GLES_WRAPPER)/inc/GLESWrapper.h \
           			$(COMMON)/inc/Common.h \
LOCAL_SRC_FILES := CubeTest.cpp \
					$(GLES_WRAPPER)/src/EGLLinuxAPIs.cpp \
					$(GLES_WRAPPER)/src/GLESAPIs.cpp \
					$(GLES_WRAPPER)/src/GLESWrapper.cpp
					
LOCAL_LDLIBS += -lGLESv2 -lEGL
include $(BUILD_SHARED_LIBRARY)
