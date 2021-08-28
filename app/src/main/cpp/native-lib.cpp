#include <jni.h>
#include <string>
#include "unununium/platform.h"

extern "C" {
    JNIEXPORT jstring JNICALL
    Java_com_uselessness_sonnydroid_MainActivity_stringFromJNI(
            JNIEnv *env,
            jobject /* this */) {
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    JNIEXPORT void JNICALL Java_com_uselessness_sonnydroid_MainActivity_screenData(
            JNIEnv *env,
            jobject thiz /* this */) {
            int ARRAY_SIZE = 320 * 240;
            int intCArray[ARRAY_SIZE];
            for (int i=0;i<ARRAY_SIZE;i++)intCArray[i] = lmao(); // 0x80FFFF00;

            jintArray intJavaArray = env->NewIntArray(ARRAY_SIZE);
            env->SetIntArrayRegion(intJavaArray, 0, ARRAY_SIZE, intCArray);

            jclass clazz = env->FindClass("com/uselessness/sonnydroid/MainActivity");
            // Calling Java Object Method ////////////////////////////////////////////
            jmethodID mid = env->GetMethodID(clazz, "setScreen", "([I)V");
            env->CallVoidMethod(thiz, mid, intJavaArray);
    }
}