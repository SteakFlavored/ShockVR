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
 *  $Source: n:/project/lib/src/2d/RCS/clpltyp.h $
 *  $Revision: 1.1 $
 *  $Author: lmfeeney $
 *  $Date: 1994/06/11 01:10:35 $
 */

#ifndef __CLPLTYPE_H
#define __CLPLTYPE_H

#include "plytyp.h"

typedef
    void *grt_line_clip_fill;

typedef
    int32_t (*grt_line_clip_fill_v) (int32_t, uintptr_t, grs_vertex *, grs_vertex *);

typedef
    int32_t (*grt_line_clip_fill_xy) (int16_t, int16_t, int16_t, int32_t, uintptr_t);

extern grt_line_clip_fill *grd_line_clip_fill_vector;

#endif
