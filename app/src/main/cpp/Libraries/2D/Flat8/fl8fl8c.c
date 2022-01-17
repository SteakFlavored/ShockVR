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
 * $Source: r:/prj/lib/src/2d/RCS/fl8fl8c.c $
 * $Revision: 1.2 $
 * $Author: kevin $
 * $Date: 1994/10/04 18:45:42 $
 *
 * Routines for drawing flat8 bitmaps into a flat8 canvas through a clut.
 *
 * This file is part of the 2d library.
 *
 * $Log: fl8fl8c.c $
 * Revision 1.2  1994/10/04  18:45:42  kevin
 * added clut fill mode specific function.  Renamed old proc.
 *
 * Revision 1.1  1994/03/15  13:15:51  kevin
 * Initial revision
 *
 */

#include <string.h>
#include "bitmap.h"
#include "cnvdat.h"
#include "fl8tf.h"

void gri_flat8_fill_clut_ubitmap (grs_bitmap *bm, int16_t x, int16_t y) {
   gri_flat8_clut_ubitmap (bm, x, y, (uint8_t *)(grd_gc.fill_parm));
}

void gri_flat8_clut_ubitmap (grs_bitmap *bm, int16_t x, int16_t y, uint8_t *cl)
{
   uint8_t *src,*dst,*srcf;
   int16_t w = bm->w;
   int16_t h = bm->h;
   int32_t ds = bm->row-w;
   int32_t dd = grd_bm.row-w;

   src = bm->bits;
   dst = grd_bm.bits + grd_bm.row*y + x;

   if (bm->flags & BMF_TRANS)
      while (h--) {
         for (srcf=src+w; src<srcf; src++,dst++)
            if ((*src)!=0) *dst=cl[*src];
         src += ds;
         dst += dd;
      }
   else
      while (h--) {
         for (srcf=src+w; src<srcf; src++,dst++) *dst=cl[*src];
         src += ds;
         dst += dd;
      }
}

