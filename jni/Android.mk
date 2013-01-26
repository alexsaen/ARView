LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libArgo

LOCAL_LDLIBS    := -lGLESv1_CM

LOCAL_SRC_FILES := native.cpp ARView.cpp 

include $(BUILD_SHARED_LIBRARY)
