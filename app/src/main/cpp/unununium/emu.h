// Copyright 2008-2010  Segher Boessenkool  <segher@kernel.crashing.org>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#ifndef _EMU_H
#define _EMU_H

#include "types.h"

#define N_MEM 0x400000

extern u16 mem[N_MEM];
void emu(void);

u32 get_ds(void);
void set_ds(u32 ds);
u16 get_video_line(void);
void emu_run();
#endif
