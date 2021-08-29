//
// Created by hacker on 23/8/21.
//
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include <malloc.h>
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
    //memcpy(rom_data, java_rom_data, size);


    //screenTest();
    //__android_log_print(ANDROID_LOG_ERROR, "SonnyDroid","%s", "Starting");
    emu();
}


void read_rom(u32 offset)
{
    memcpy(mem+0x4000, rom_data + (offset + 0x4000), 2*(N_MEM - 0x4000) );

    /*JNIEnv* env;
    (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL); // check error etc
    jclass clazz = (*env)->FindClass(env, "com/uselessness/sonnydroid/Emulator");
    jmethodID mid = (*env)->GetMethodID(env,clazz, "getRomBytes", "(II)[B");

    u32 copy_amt = 2*(N_MEM - 0x4000); u32 total_offset = (offset + 0x4000);
    jobject array = (*env)->CallObjectMethod(env, emulatorObj, mid, (jint)total_offset, (jint)copy_amt);
    u16 * m;
    m = (*env)->GetByteArrayElements(env, array, NULL);
    (*env)->ReleaseByteArrayElements(env, array, m, JNI_ABORT);
    memcpy(mem+0x4000, m+total_offset, copy_amt);
    */
    //delete[] rom_data;
}

JNIEXPORT jintArray JNICALL Java_com_uselessness_sonnydroid_Emulator_getScreen(
        JNIEnv *env, jobject thiz /* this */) {
    int ARRAY_SIZE = 320 * 240;

    for (int i=0;i<ARRAY_SIZE;i++)vscreen[i] = lmao(); // 0x80FFFF00;

    jintArray intJavaArray = (*env)->NewIntArray(env,ARRAY_SIZE);
    (*env)->SetIntArrayRegion(env, intJavaArray, 0, ARRAY_SIZE, vscreen);
    return intJavaArray;
}

