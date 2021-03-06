// Copyright 2008-2010  Segher Boessenkool  <segher@kernel.crashing.org>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>

#include "types.h"
#include "emu.h"
#include "video.h"
#include "audio.h"
#include "render.h"

#include "platform.h"
//#include <android/log.h>

#define PIXEL_SIZE 3



void open_rom(const char *path)
{
	rom_file = fopen(path, "rb");
	//if (!rom_file)
	//	__android_log_print(ANDROID_LOG_DEBUG, "e", "\n Cannot read ROM file \n");
	//	fatal("Cannot read ROM file %s\n", path);
    /*fseek(rom_file, 0, SEEK_END);          // Jump to the end of the file
    long filelen;
    filelen = ftell(rom_file);             // Get the current byte offset in the file
    printf("Filelength %i", filelen);
    rewind(rom_file);                      // Jump back to the beginning of the file

    rom_data = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(rom_data, filelen, 1, rom_file); // Read in the entire file
    fclose(rom_file); // Close the file
    */
}

void read_rom(u32 offset)
{
	
	//u32 n;

	//fseek(rom_file, 2*(offset + 0x4000), SEEK_SET);
	//n = fread(mem + 0x4000, 2, N_MEM - 0x4000, rom_file);
	
	int size_e = 2;
	int nmemb = (N_MEM - 0x4000);
	//for (int i=0;i<no_elements;i++){*(mem + 0x4000) = (u16)*(rom_data + 2*(offset + 0x4000));}
    //memcpy(mem + 0x4000, rom_data+ 2*(offset + 0x4000), nmemb);
}


void *open_eeprom(const char *name, u8 *data, u32 len)
{
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

	return fp;
}

void save_eeprom(void *cookie, u8 *data, u32 len)
{
	FILE *fp = cookie;

	rewind(fp);

	fwrite(data, 1, len, fp);
	fflush(fp);
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

void update_screen(void)
{	
	printf("Update Screen");
	FILE *wf;
	wf = fopen("/tmp/screen_data", "wb");
	
	//u32 pitch = 100;//sdl_surface->pitch / 4;
	//u32 *pixels 
	
	u32 x, y, j;
	for (y = 0; y < 240; y++) {
		/*u32 *p = pixels + PIXEL_SIZE*y*pitch;*/
		u32 *s = screen + 320*y;
		

		for (x = 0; x < 320; x++) {
			u32 c = *s++; //Colout of current pixel
                        //virt_screen[x][y]
            vscreen[320*y+x] = c;
			fprintf(wf, "%i\n",vscreen[320*y+x]);
		}
	}
	
	fclose(wf);
	
	/*
	if (SDL_MUSTLOCK(sdl_surface))
		if (SDL_LockSurface(sdl_surface) < 0)
			fatal("oh crap\n");
	
	u32 pitch = sdl_surface->pitch / 4;
	u32 *pixels = sdl_surface->pixels;

	
	/*
	if (SDL_MUSTLOCK(sdl_surface))
		SDL_UnlockSurface(sdl_surface);

	SDL_UpdateRect(sdl_surface, 0, 0, 0, 0);
	*/
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

static void handle_controller_key(int key, int down)
{/*
	switch (key) {
	case SDLK_UP: case 'j':
		button_up = down;
		break;
	case SDLK_DOWN: case 'm':
		button_down = down;
		break;
	case SDLK_LEFT: case 'n':
		button_left = down;
		break;
	case SDLK_RIGHT: case ',':
		button_right = down;
		break;
	case SDLK_SPACE: case 'h':
		button_A = down;
		break;
	case 'b': case 'k':
		button_B = down;
		break;
	case 'g':
		button_C = down;
		break;
	case 'l':
		button_menu = down;
		break;
	}*/
}

char update_controller(void)
{/*
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

	return 0;
	*/
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
	exit(1);
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
