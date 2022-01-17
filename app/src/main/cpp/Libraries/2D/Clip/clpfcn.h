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
 * $Source: n:/project/lib/src/2d/RCS/clpfcn.h $
 * $Revision: 1.3 $
 * $Author: lmfeeney $
 * $Date: 1994/06/11 00:46:51 $
 *
 * Routines for clipping vertex pointer polygons to a rectangle.
 *
 * This file is part of the 2d library.
 *
 * $Log: clpfcn.h $
 * Revision 1.3  1994/06/11  00:46:51  lmfeeney
 * gr_clip_fix_code is now used in mulitple 2d files, but probably
 * doesn't need to be external to 2d.h (oh, well)
 *
 * Revision 1.2  1993/10/19  09:59:01  kaboom
 * Fixed #ifndef check macro, now includes grs.h.
 *
 * Revision 1.1  1993/10/01  15:34:00  kaboom
 * Initial revision
 */

#ifndef __CLPFCN_H
#define __CLPFCN_H
#include "grs.h"
#include "plytyp.h"


/* prototypes for analytic clippers. */
extern int32_t gr_clip_fix_code
   (fix, fix);
extern int32_t gr_clip_int_line
   (int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1);
extern int32_t gr_clip_fix_line
   (int32_t *x0, int32_t *y0, int32_t *x1, int32_t *y1);
extern int32_t gr_clip_fix_poly
   (int32_t n, fix *vlist, fix *clist);
extern int32_t gr_clip_poly
   (int32_t n, int32_t l, grs_vertex **vplist, grs_vertex ***pcplist);
extern int32_t gr_clip_spoly
   (int32_t n, fix *vlist, fix *clist, fix *ilist, fix *cilist);
extern int32_t gr_clip_fix_cpoly
   (int32_t n, fix *vlist, grs_rgb *blist, fix *clist, grs_rgb *cblist);
extern int32_t gr_clip_rect
   (int16_t *left, int16_t *top, int16_t *right, int16_t *bot);
extern int32_t gr_clip_mono_bitmap
   (grs_bitmap *bm, int16_t *x, int16_t *y);
extern int32_t gr_clip_flat8_bitmap
   (grs_bitmap *bm, int16_t *x, int16_t *y);
extern int32_t gr_clip_flat24_bitmap
   (grs_bitmap *bm, int16_t *x, int16_t *y);
#endif /* !__CLPFCN_H */
