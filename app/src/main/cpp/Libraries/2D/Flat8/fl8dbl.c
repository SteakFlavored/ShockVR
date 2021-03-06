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
//
// $Source: r:/prj/lib/src/2d/RCS/fl8dbl.asm $
// $Revision: 1.3 $
// $Author: kevin $
// $Date: 1994/09/08 00:00:18 $
//
// Bitmap doubling primitives.
//

#include "dbg.h"
#include "grs.h"
#include "cnvdat.h"
#include "flat8.h"
#include "blndat.h"

// ------------------------------------------------------------------------
// PowerPC routines
// ------------------------------------------------------------------------
// ========================================================================
void flat8_flat8_h_double_ubitmap(grs_bitmap *bm)
{
    Warning("call mark");
/*     int32_t        h,v,endh,endv;
     uint8_t *src=bm->bits, *dst=grd_bm.bits;
     int32_t    srcAdd,dstAdd;
     uint8_t    temp;

     srcAdd = bm->row-bm->w;
     dstAdd = grd_bm.row - (bm->w<<1);
     endh = bm->w;
     endv = bm->h;

     for (v=0; v<endv; v++)
      {
          for (h=0; h<endh; h++)
           {
               temp = *(src++);
               *(dst++) = temp;
               *(dst++) = temp;
           }

          src+=srcAdd;
          dst+=dstAdd;
      }*/
}


// ========================================================================
void flat8_flat8_smooth_h_double_ubitmap(grs_bitmap *srcb, grs_bitmap *dstb)
 {
     int32_t            h,v,endh,endv;
     uint8_t     *src=srcb->bits, *dst=dstb->bits;
     int32_t        srcAdd,dstAdd;
    uint16_t    curpix,tempshort;
    uint8_t     *local_grd_half_blend;

    local_grd_half_blend = grd_half_blend;
     if (!local_grd_half_blend) return;

     srcAdd = (srcb->row-srcb->w)-1;
     dstAdd = dstb->row - (srcb->w<<1);
     endh = srcb->w-1;
     endv = srcb->h;

     for (v=0; v<endv; v++)
      {
          curpix = * (int16_t *) src;
          src+=2;
          for (h=0; h<endh; h++)
           {
               tempshort = curpix & 0xff00;
               tempshort |= local_grd_half_blend[curpix];
               * (uint16_t *) dst = tempshort;
               dst += 2;
               curpix = (curpix<<8) | *(src++);
           }

          // double last pixel
          curpix>>=8;
          *(dst++) = curpix;
          *(dst++) = curpix;

          src+=srcAdd;
          dst+=dstAdd;
      }
 }

// ========================================================================
// src = eax, dest = edx
void flat8_flat8_smooth_hv_double_ubitmap(grs_bitmap *src, grs_bitmap *dst)
 {
     int32_t             tempH, tempW, temp, savetemp;
     uint8_t        *srcPtr,*dstPtr;
     uint8_t        *shvd_read_row1, *shvd_write, *shvd_read_row2, *shvd_read_blend;
     uint16_t    tempc;


     dst->row <<= 1;
     flat8_flat8_smooth_h_double_ubitmap(src,dst);

    dst->row = tempW = dst->row >> 1;
    dstPtr = dst->bits;

    tempH = src->h-1;
    temp =  src->w << 1;
    dstPtr += temp;
    temp = -temp;

    shvd_read_row1 = dstPtr;
    dstPtr += tempW;
    shvd_write = dstPtr - 1;
    dstPtr += tempW;
    shvd_read_row2 = dstPtr;
    shvd_read_blend = grd_half_blend;
    savetemp = temp;

    do
     {
        do
         {
             tempc = shvd_read_row1[temp];
             tempc |= ((uint16_t) shvd_read_row2[temp]) << 8;
             temp++;

             shvd_write[temp] = shvd_read_blend[tempc];
         }
        while (temp!=0);

        if (--tempH==0) break;

        shvd_read_row1 = dstPtr;
        dstPtr += tempW;
        shvd_write = dstPtr - 1;
        dstPtr += tempW;
        shvd_read_row2 = dstPtr;
        temp = savetemp;
     }
    while (true);

    // do last row
    srcPtr = dstPtr + savetemp;
    dstPtr += tempW + savetemp;
    savetemp = -savetemp;

    for (;savetemp>0; savetemp--)
      *(dstPtr++) = *(srcPtr++);
 }
