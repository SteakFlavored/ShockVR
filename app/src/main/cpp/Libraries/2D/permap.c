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
 * $Source: r:/prj/lib/src/2d/RCS/permap.c $
 * $Revision: 1.8 $
 * $Author: kevin $
 * $Date: 1994/12/01 14:59:57 $
 *
 * Full perspective texture mapping dispatchers.
 *
*/

#include "bitmap.h"
#include "buffer.h"
#include "clpcon.h"
#include "clpfcn.h"
#include "cnvdat.h"
#include "fill.h"
#include "fl8p.h"
#include "ifcn.h"
#include "grnull.h"
#include "pertyp.h"
#include "scrmac.h"
#include "tmapfcn.h"
#include "tmaps.h"
#include "tmaptab.h"

fix fix_div_16_16_3(fix a, fix b)
{
    int64_t result = ((int64_t)a << 29L) / (int64_t)b;
    return fix_from_int64_lo(result);
}

// PowerPC math routines used by permap stuff
#if defined(powerc) || defined(__powerc)
#else
asm fix fix_div_16_16_3 (fix a, fix b)
 {
     tst.l        8(a7)
    beq.s        @DivZero

     move.l    4(a7),d0
     move.l    d0,d1
     asr.l        #3,d1
     moveq        #29,d2
     lsl.l        d2,d0
    dc.w        0x4C6F,0x0C01,0x0008        //     divs.l    8(a7),d1:d0
    bvs.s        @DivZero
     rts

@DivZero:
    move.l    #0x7FFFFFFF,d0
    tst.b        4(a7)
    bpl.s        @noNeg
    neg.l        d0
@noNeg:
    rts
 }

asm fix fix_mul_3_3_3 (fix a, fix b)
 {
     move.l    4(A7),d0
    dc.w        0x4c2f,0x0c01,0x0008        //     muls.l    8(A7),d1:d0
    moveq        #29,d2
    lsr.l        d2,d0
    lsl.l        #3,d1
    or.l        d1,d0
    rts
 }


asm fix fix_mul_3_32_16 (fix a, fix b)
 {
     move.l    4(A7),d0
    dc.w        0x4c2f,0x0c01,0x0008        //     muls.l    8(A7),d1:d0
    moveq        #13,d2
    lsr.l        d2,d0
    moveq        #19,d2
    lsl.l        d2,d1
    or.l        d1,d0
    rts
 }


asm fix fix_mul_3_16_20 (fix a, fix b)
 {
     move.l    4(A7),d0
    dc.w        0x4c2f,0x0c01,0x0008        //     muls.l    8(A7),d1:d0
    asr.l        #1,d1
    move.l    d1,d0
    rts
 }


asm fix fix_mul_16_32_20 (fix a, fix b)
 {
     move.l    4(A7),d0
    dc.w        0x4c2f,0x0c01,0x0008        //     muls.l    8(A7),d1:d0
    lsr.l        #4,d0
    moveq        #28,d2
    lsl.l        d2,d1
    or.l        d1,d0
    rts
 }
#endif

extern int32_t gri_per_umap_setup(int32_t n, grs_vertex **vpl, grs_per_setup *ps);

bool grd_enable_quad_blend=false;

int32_t per_map (grs_bitmap *bm, int32_t n, grs_vertex **vpl, grs_tmap_info *ti)
{
    grs_vertex **cpl;             /* clipped vertices */
    int32_t m;                            /* number of clipped vertices */

    cpl = NULL;
    m = gr_clip_poly(n,5,vpl,&cpl);
    if (m>2)
        per_umap(bm,m,cpl,ti);
    gr_free_temp(cpl);

    return ((m>2) ? CLIP_NONE : CLIP_ALL);
}

void per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl, grs_tmap_info *ti)
{
    int16_t percode;
    grs_per_setup ps;
    uint8_t *save_bits;

    ps.dp=bm->flags&BMF_TRANS;
    if (2*grd_gc.fill_type + ps.dp==2*FILL_SOLID) {
        h_umap(bm, n, vpl, ti);
        return;
    }
//ÊMLA - doesnt' ever appear to use this
/*
    if ((bm->type==BMT_FLAT8)&&grd_enable_quad_blend) {
        int32_t u_min=vpl[0]->u;
        int32_t u_max=u_min;
        int32_t v_min=vpl[0]->v;
        int32_t v_max=v_min;
        int32_t x_min=vpl[0]->x;
        int32_t x_max=x_min;
        int32_t y_min=vpl[0]->y;
        int32_t y_max=y_min;
        int32_t i,canvas_delta;

        for (i=1;i<n;i++) {
            if (vpl[i]->u>u_max) u_max=vpl[i]->u;
            if (vpl[i]->u<u_min) u_min=vpl[i]->u;
            if (vpl[i]->v>v_max) v_max=vpl[i]->v;
            if (vpl[i]->v<v_min) v_min=vpl[i]->v;
        }

        for (i=1;i<n;i++) {
            if (vpl[i]->y>y_max) y_max=vpl[i]->y;
            if (vpl[i]->y<y_min) y_min=vpl[i]->y;
            if (vpl[i]->x>x_max) x_max=vpl[i]->x;
            if (vpl[i]->x<x_min) x_min=vpl[i]->x;
        }
        if (x_max-x_min>y_max-y_min)
            canvas_delta=x_max-x_min;
        else
            canvas_delta=y_max-y_min;
        if (canvas_delta>30*FIX_UNIT) {
            if ((fix_int(u_max)-fix_int(u_min)<4)&&
                 (fix_int(v_max)-fix_int(v_min)<4)) {
                extern void gri_flat8_hv_quadruple_sub_bitmap(grs_bitmap *sbm, grs_bitmap *dbm, int32_t u, int32_t v);
                int32_t u=fix_int(u_min);
                int32_t v=fix_int(v_min);
                grs_bitmap tbm;

                if (u+4>=bm->w) u=bm->w-4;
                if (v+4>=bm->h) v=bm->h-4;
                tbm.type=BMT_FLAT8;
                tbm.flags=bm->flags;
                gri_flat8_hv_quadruple_sub_bitmap(bm, &tbm, u, v);
                for (i=0;i<n;i++) {
                    vpl[i]->u = ((vpl[i]->u-fix_make(u,0))<<2)+8*FIX_UNIT;
                    vpl[i]->v = ((vpl[i]->v-fix_make(v,0))<<2)+8*FIX_UNIT;
                }
                per_umap(&tbm, n, vpl, ti);
                for (i=0;i<n;i++) {
                    vpl[i]->u = ((vpl[i]->u-8*FIX_UNIT)>>2)+fix_make(u,0);
                    vpl[i]->v = ((vpl[i]->v-8*FIX_UNIT)>>2)+fix_make(v,0);
                }
                return;
            }
        }
    }
*/

    percode=gri_per_umap_setup(n, vpl, &ps);

    /* should be set by init func, but just in case...*/
    ps.shell_func=gr_null;

    ps.dp+=ti->tmap_type+(GRD_FUNCS*bm->type);
    if (grd_gc.fill_type!=FILL_NORM)
        ps.fill_parm=grd_gc.fill_parm;
    else if (ti->flags&TMF_CLUT)
        if ((ps.clut=ti->clut)==NULL)
            ps.clut=gr_get_clut();

    save_bits=bm->bits;      /* in case bitmap type is rsd8 */

    switch (percode) {
    case GR_PER_CODE_BIGSLOPE:
          ((void (*)(grs_bitmap *, grs_per_setup *))(grd_tmap_hscan_init_table[ps.dp]))(bm,&ps);
        ((void (*)(grs_bitmap *, int32_t, grs_vertex **, grs_per_setup *))(ps.shell_func))(bm,n,vpl,&ps);
        break;
    case GR_PER_CODE_SMALLSLOPE:
        ((void (*)(grs_bitmap *, grs_per_setup *))(grd_tmap_vscan_init_table[ps.dp]))(bm,&ps);
        ((void (*)(grs_bitmap *, int32_t, grs_vertex **, grs_per_setup *))(ps.shell_func))(bm,n,vpl,&ps);
        break;
    case GR_PER_CODE_LIN:
        ti->tmap_type+=GRC_BILIN-GRC_PER;
        h_umap(bm,n,vpl,ti);
        break;
    case GR_PER_CODE_FLOOR:
        ti->tmap_type+=GRC_FLOOR-GRC_PER;
        ti->flags|=TMF_FLOOR;
        h_umap(bm,n,vpl,ti);
        break;
    case GR_PER_CODE_WALL:
        ti->tmap_type+=GRC_WALL2D-GRC_PER;
        ti->flags|=TMF_WALL;
        v_umap(bm,n,vpl,ti);
        break;
    }
    bm->bits=save_bits;
    return;
}


