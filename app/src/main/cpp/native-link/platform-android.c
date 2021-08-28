//
// Created by hacker on 25/8/21.
//

// Copyright 2008-2010  Segher Boessenkool  <segher@kernel.crashing.org>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>

#include "../unununium/types.h"
#include "../unununium/emu.h"
#include "../unununium/video.h"
#include "../unununium/audio.h"
#include "../unununium/render.h"

#include "../unununium/platform.h"
#include <android/log.h>
#include <stdbool.h>
#include "platform-android.h"

#define PIXEL_SIZE 3



void open_rom(const char *path)
{
    //rom_file = fopen(path, "rb");
    //if (!rom_file)fatal("Cannot read ROM file %s\n", path);
}
/*
void read_rom(u32 offset)
{
	//u32 n;
	//fseek(rom_file, 2*(offset + 0x4000), SEEK_SET);
	//n = fread(mem + 0x4000, 2, N_MEM - 0x4000, rom_file);
    int total_offset = 2*(offset + 0x4000);
    memcpy(mem + 0x4000, rom_data + total_offset, N_MEM - 0x4000);
}*/


void *open_eeprom(const char *name, u8 *data, u32 len)
{/*
    printf("Opening EEPROM");
    char path[256];
    char *home;
    FILE *fp;

    memset(data, 0, len);

    home = getenv("HOME");
    if (!home)
        fatal("cannot find HOME\n");
    snprintf(path, sizeof path, "%s/.unununium", home);
    mkdir(path, 0777);
    snprintf(path, sizeof path, "%s/.unununium/%s.eeprom", home, name);

    fp = fopen(path, "rb");
    if (fp) {
        fread(data, 1, len, fp);
        if (ferror(fp))
            fatal("error reading EEPROM file %s\n", path);
        fclose(fp);
    }

    fp = fopen(path, "wb");
    if (!fp)
        fatal("cannot open EEPROM file %s\n", path);

    fwrite(data, 1, len, fp);
    fflush(fp);

    return fp;*/
}

void save_eeprom(void *cookie, u8 *data, u32 len)
{
    /*
    FILE *fp = cookie;

    rewind(fp);

    fwrite(data, 1, len, fp);
    fflush(fp);*/
}

static inline u8 x58(u32 x)
{
    x &= 31;
    return (x << 3) | (x >> 2);
}

u32 get_colour(u8 r, u8 g, u8 b)
{
    return 256*256*r + 256*g + b;//SDL_MapRGB(sdl_surface->format, r, g, b);
}

void render_palette(void)
{
    u32 i;
    for (i = 0; i < 256; i++) {
        u16 p = mem[0x2b00 + i];
        palette_rgb[i] = get_colour(x58((p >> 10) & 31),
                                    x58((p >> 5) & 31),
                                    x58(p & 31));
        /*SDL_MapRGB(sdl_surface->format,
		                            x58((p >> 10) & 31),
		                            x58((p >> 5) & 31),
		                            x58(p & 31));*/
    }

}

int ARRAY_SIZE = 320 * 240;
jintArray intJavaArrayScreen;
void update_screen(void)
{
    u32 x, y, j;
    for (y = 0; y < 240; y++) {
        /*u32 *p = pixels + PIXEL_SIZE*y*pitch;*/
        u32 *s = screen + 320*y;

        for (x = 0; x < 320; x++) {
            u32 c = *s++; //Colour of current pixel
            vscreen[320*y+x] = c;
        }

    }

    // Call screen update function

    JNIEnv* env;
    (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL); // check error etc
    jclass clazz = (*env)->FindClass(env, "com/uselessness/sonnydroid/Emulator");
    jmethodID mid = (*env)->GetMethodID(env,clazz, "updateScreenView", "([I)V");

    if (intJavaArrayScreen == NULL){
        intJavaArrayScreen = (*env)->NewIntArray(env,ARRAY_SIZE);
    }
    (*env)->SetIntArrayRegion(env, intJavaArrayScreen, 0, ARRAY_SIZE, vscreen);
    (*env)->CallVoidMethod(env, emulatorObj, mid, intJavaArrayScreen);
}
u8 button_up;
u8 button_down;
u8 button_left;
u8 button_right;
u8 button_A;
u8 button_B;
u8 button_C;
u8 button_menu;

static char handle_debug_key(int key)
{/*
	switch (key) {
	case SDLK_ESCAPE:
		return 0x1b;
	case '0' ... '8':
	case 't':
	case 'y':
	case 'u':
	case 'x':
	case 'q':
	case 'w': case 'e':
	case 'a': case 's': case 'd':
	case 'f':
	case 'v': case 'c':
		return (key);
	}

	return 0;*/
}


char update_controller(void)
{

    JNIEnv* env;
    (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL); // check error etc

    jclass clazz = (*env)->FindClass(env, "com/uselessness/sonnydroid/Emulator");
    jmethodID mid = (*env)->GetMethodID(env, clazz, "getInputs", "()[Z");
    jbooleanArray inputs_java = (*env)->CallObjectMethod(env, emulatorObj, mid);
    bool *inputs;
    inputs = (*env)->GetBooleanArrayElements(env, inputs_java, NULL);
    //jsize size = (*env)->GetArrayLength(env, inputs_java);
    (*env)->ReleaseBooleanArrayElements(env, inputs_java, inputs, JNI_ABORT);

    button_up = inputs[0];
    button_down = inputs[1];
    button_left = inputs[2];
    button_right = inputs[3];
    button_A = inputs[4];
    button_B = inputs[5];
    button_C = inputs[6];
    button_menu = inputs[7];
    //__android_log_print(ANDROID_LOG_ERROR, "SonnyDroid","Buttons: %i %i %i %i %i %i %i %i",
    //                    inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7]);

    /*
	SDL_Event event;
	char key;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_KEYDOWN:
			key = handle_debug_key(event.key.keysym.sym);
			if (key)
				return (key);

			// Fallthrough.
		case SDL_KEYUP:
			handle_controller_key(event.key.keysym.sym,
			                      (event.type == SDL_KEYDOWN));
			break;

		case SDL_QUIT:
			return 0x1b;

		case SDL_ACTIVEEVENT:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			continue;

		default:
			warn("Unknown SDL event type %d\n", event.type);
			continue;
		}
	}
    */
	return 0;

}


u32 get_realtime(void)
{
    struct timeval tv;

    gettimeofday(&tv, 0);
    return 1000000*tv.tv_sec + tv.tv_usec;
}


void warn(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
}

#ifdef USE_DEBUG
void debug(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
}
#endif

void fatal(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    __android_log_print(ANDROID_LOG_ERROR, "SonnyDroid","%s", format);

    //exit(1);
}


static void mix(__unused void *cookie, u8 *data, int n)
{
    if (mute_audio) {
        memset(data, 0, n);
        return;
    }

    //audio_render((s16 *)data, n/2);
}


void platform_init(void)
{
}
