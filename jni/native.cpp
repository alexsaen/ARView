/*
	Copyright (c) 2013, Alexey Saenko
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include <jni.h>
#include "ARView.h"

#define JAVA_NATIVE_CLASS "com/androidemo/arview/ARViewSurfaceView$Engine"

static ARView arview;

namespace native {

JNIEXPORT void JNICALL draw(JNIEnv *, jobject) {
	arview.draw();
}

JNIEXPORT void JNICALL reshape(JNIEnv *env, jobject obj, jint width, jint height) {
	arview.reshape(width, height);
}

JNIEXPORT void JNICALL accelerometerChanged(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z) {
	arview.accelerometerChanged(vec3(x, y, z));
}

JNIEXPORT void JNICALL magnetometerChanged(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z) {
	arview.magnetometerChanged(vec3(x, y, z));
}

JNIEXPORT void JNICALL gyroscopeChanged(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z, jfloat dt) {
	arview.gyroscopeChanged(vec3(x, y, z), dt);
}

static JNINativeMethod methods[] = {
	 { "draw",					"()V", 		(void*)draw 					},
	 { "reshape", 				"(II)V", 	(void*)reshape  				},
	 { "accelerometerChanged",	"(FFF)V", 	(void*)accelerometerChanged  	},
	 { "magnetometerChanged",	"(FFF)V", 	(void*)magnetometerChanged  	},
	 { "gyroscopeChanged",		"(FFFF)V", 	(void*)gyroscopeChanged  		},
};

}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env = 0;
	if(vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
		return -1;

	jclass classNative = env->FindClass(JAVA_NATIVE_CLASS);
	if(!classNative)
		return -1;

	if(env->RegisterNatives(classNative, native::methods, sizeof(native::methods)/sizeof(native::methods[0])) < 0)
		return -1;

	return JNI_VERSION_1_4;
}

