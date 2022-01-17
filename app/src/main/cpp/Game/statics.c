/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
/*
 * $Source: r:/prj/cit/src/RCS/statics.c $
 * $Revision: 1.8 $
 * $Author: xemu $
 * $Date: 1994/11/01 09:18:31 $
 *
 * static variables and the people who love them
 */

// the idea is you can take just obj mem, tmap and obj, or both
// + either big buffer or the frame buffer or both

// still need to have static.h for people to use this stuff...

// how do we get this aligned right.
// perhaps do this file in asm for real?
// if we did it in asm, and had these point, then we could use
// labels for top and bottom, which would be good...
// sadly, this alphabetizes, since it is so cool

// put big buffer here? and have a define after it


#include "textmaps.h"
uint8_t         tmap_static_mem[NUM_STATIC_TMAPS*SIZE_STATIC_TMAP];
#ifdef SVGA_CUTSCENES
uint8_t         tmap_big_buffer[NUM_STATIC_TMAPS * SIZE_BIG_TMAP];
#endif

#include "objects.h"
#include "objapp.h"
Obj            objs[NUM_OBJECTS];
ObjRef        objRefs[NUM_REF_OBJECTS];
uint8_t         objsDealt[NUM_OBJECTS/8];

// put rest of obj system here, define after it

#include "mfddims.h"
#define FRAME_BUFFER_SIZE  (320*200) + 4096
uint8_t         frameBuffer[FRAME_BUFFER_SIZE];
#define WACKY_SVGA_MFD_SIZE    52744
uint8_t         frameBuffer2[WACKY_SVGA_MFD_SIZE];

uint8_t *mfd_canvas_bits = frameBuffer2;

#define ALTERNATE_BUFFER_SIZE     ((MFD_VIEW_HGT * MFD_VIEW_WID) + FRAME_BUFFER_SIZE)

#include "map.h"
#define STATIC_MAP_SIZE     16 << (DEFAULT_XSHF + DEFAULT_YSHF)

uint8_t static_map[STATIC_MAP_SIZE];

#include "objprop.h"

#define OBJ_BITMAP_POOL_SIZE ((NUM_OBJECT * 2) + 230)
grs_bitmap obj_bitmap_pool[OBJ_BITMAP_POOL_SIZE];

#include "svgacurs.h"
uint8_t svga_cursor_bits[SVGA_CURSOR_WIDTH * SVGA_CURSOR_HEIGHT];
grs_bitmap svga_cursor_bmp;

#define MAX_OPT_WID  154
#define MAX_OPT_HT    58
uint8_t svga_options_cursor_bits[MAX_OPT_WID * MAX_OPT_HT];
