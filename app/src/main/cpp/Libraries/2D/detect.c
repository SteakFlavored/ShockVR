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
 * $Source: r:/prj/lib/src/2d/RCS/detect.c $
 * $Revision: 1.11 $
 * $Author: kevin $
 * $Date: 1994/08/16 13:16:57 $
 *
 * Routine to detect what kind of video card is present and which
 * graphics modes are available.
 *
 * This file is part of the 2d library.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MLA #include <datapath.h"
#include "grs.h"
#include "detect.h"
#include "bitmap.h"
#include "cnvtab.h"
#include "devtab.h"
#include "idevice.h"
#include "mode.h"
#include "cnvtab.h"
#include "tabdat.h"

// extern
extern void (**grd_device_table_list[])();

// ======================================================================
// Mac version of gr_detect
int32_t gr_detect(grs_sys_info *info)
 {
    /* default to 640x480x8 standard Mac res */
    info->id_maj = 0;
    info->id_min = 0;
    info->memory = 300;
    info->modes[0] = GRM_640x480x8;
    info->modes[1] = -1;
    info->modes[2] = -1;
    info->modes[3] = -1;
    info->modes[4] = -1;

  grd_device_table = grd_device_table_list[info->id_maj];
  grd_canvas_table_list[BMT_DEVICE] = (void (**)())grd_device_table[GRT_CANVAS_TABLE];

    return(0);
 }
