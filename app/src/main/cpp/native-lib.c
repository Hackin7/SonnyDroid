//
// Created by hacker on 23/8/21.
//
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include "unununium/platform.h"
#include "unununium/emu.h"
#include "unununium/board.h"
#include "unununium/io.h"
#include "unununium/video.h"
#include "unununium/audio.h"
#include "unununium/emu.h"
#include "native-link/platform-android.h"
//#include <string>

void screenTest(){
    JNIEnv* env;
    (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL); // check error etc
    int ARRAY_SIZE = 320 * 240;
    for (int i=0;i<ARRAY_SIZE;i++)vscreen[i] = 0x8000FF00;
    jintArray intJavaArray = (*env)->NewIntArray(env,ARRAY_SIZE);
    (*env)->SetIntArrayRegion(env, intJavaArray, 0, ARRAY_SIZE, vscreen);

    jclass clazz = (*env)->FindClass(env, "com/uselessness/sonnydroid/Emulator");
    jmethodID mid = (*env)->GetMethodID(env,clazz, "updateScreenView", "([I)V");
    (*env)->CallVoidMethod(env, emulatorObj, mid, intJavaArray);
}

JNIEXPORT void JNICALL Java_com_uselessness_sonnydroid_Emulator_nativeInit(JNIEnv *env, jobject thiz, jbyteArray array) {
    g_jvm = 0;
    (*env)->GetJavaVM(env, &g_jvm);
    emulatorObj = (*env)->NewGlobalRef(env, thiz);

    // Load ROM
    rom_data = (*env)->GetByteArrayElements(env, array, NULL);
    jsize size = (*env)->GetArrayLength(env, array);
    (*env)->ReleaseByteArrayElements(env, array, rom_data, JNI_ABORT);

/*
    jclass clazz = (*env)->FindClass(env, "com/uselessness/sonnydroid/Emulator");
    jmethodID mid = (*env)->GetMethodID(env,clazz, "updateScreenView", "()V");
    (*env)->CallVoidMethod(env, emulatorObj, mid);*/
    screenTest();
    __android_log_print(ANDROID_LOG_ERROR, "SonnyDroid","%s", "Starting");
    emu();
    __android_log_print(ANDROID_LOG_ERROR, "SonnyDroid","%s", "End");
    screenTest();
}


void read_rom(u32 offset)
{
    memcpy(mem+0x4000, rom_data + (offset + 0x4000), 2*(N_MEM - 0x4000) );
}

JNIEXPORT jintArray JNICALL Java_com_uselessness_sonnydroid_Emulator_getScreen(
        JNIEnv *env, jobject thiz /* this */) {
    int ARRAY_SIZE = 320 * 240;

    for (int i=0;i<ARRAY_SIZE;i++)vscreen[i] = lmao(); // 0x80FFFF00;

    jintArray intJavaArray = (*env)->NewIntArray(env,ARRAY_SIZE);
    (*env)->SetIntArrayRegion(env, intJavaArray, 0, ARRAY_SIZE, vscreen);
    return intJavaArray;
}

JNIEXPORT void JNICALL Java_com_uselessness_sonnydroid_MainActivity_screenData(
        JNIEnv *env, jobject thiz /* this */) {
    int ARRAY_SIZE = 320 * 240;

    for (int i=0;i<ARRAY_SIZE;i++)vscreen[i] = lmao(); // 0x80FFFF00;

    jintArray intJavaArray = (*env)->NewIntArray(env,ARRAY_SIZE);
        (*env)->SetIntArrayRegion(env, intJavaArray, 0, ARRAY_SIZE, vscreen);

    jclass clazz = (*env)->FindClass(env, "com/uselessness/sonnydroid/MainActivity");
    // Calling Java Object Method ////////////////////////////////////////////
    jmethodID mid = (*env)->GetMethodID(env,clazz, "setScreen", "([I)V");
        (*env)->CallVoidMethod(env,thiz, mid, intJavaArray);
}
