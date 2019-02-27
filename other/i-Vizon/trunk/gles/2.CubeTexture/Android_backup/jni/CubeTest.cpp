#include "com_ivizon_cubetest_NativeLib.h"

iv::GLESWrapper *cubeTexture = new CubeTexture();

JNIEXPORT void JNICALL Java_com_ivizon_cubetest_NativeLib_init
  (JNIEnv *, jclass, jint, jint) {
	cubeTexture->initGLES();

}
JNIEXPORT void JNICALL Java_com_ivizon_cubetest_NativeLib_step
  (JNIEnv *, jclass) {
	cubeTexture->startRender();
efstrt
}
