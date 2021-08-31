// Copyright 2008,2009  Segher Boessenkool  <segher@kernel.crashing.org>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#include <stdio.h>
#include <string.h>
#include <time.h>


#include "types.h"
#include "platform.h"
#include "emu.h"
#include "dialog.h"


#include "disas.h"
#include "timer.h"
#include "video.h"
#include "audio.h"
#include "io.h"
#include "platform.h"
#include "render.h"
#include "board.h"


int main(int argc, char *argv[])
{

	if (0)
		;
	else {
		if (argc != 2)
			fatal("usage: %s <rom-file>\n", argv[0]);

		open_rom(argv[1]);
	}
	srandom(time(0));
	emu();
	return 0;
}

int lmao(){
    return 0x80FF0000;
}
