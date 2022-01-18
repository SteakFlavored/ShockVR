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
// $Source: r:/prj/lib/src/2d/RCS/fl8s.asm $
// $Revision: 1.1 $
// $Author: kevin $
// $Date: 1994/08/16 12:34:32 $
//
// Inner loops of scaling and clut scaling primitives.
//
// This file is part of the 2d library.
//

#include "tmapint.h"
#include "flat8.h"
#include "cnvdat.h"
#include "grnull.h"
#include "gente.h"
#include "poly.h"
#include "grpix.h"

// globals
int32_t    ADD_DEST_OFF;
int32_t    ADD_DV_FRAC_OFF;
int32_t    AND_BM_ROW_OFF;
int32_t    ADD_SRC_OFF;
int32_t    SET_REPS_OFF;
int32_t    SET_OFFSET_OFF;
int32_t    JMP_LOOP_MIDDLE_OFF;

#define unroll_num 4
#define unroll_log 2

// externs
extern int32_t gri_poly_loop (grs_tmap_loop_info *ti);

// internal prototypes
int32_t gri_scale_umap_loop_C(grs_tmap_loop_info *tli);

// ========================================================================
// opaque solid polygon scaler
int32_t gri_opaque_solid_scale_umap_init(grs_tmap_loop_info *info, grs_vertex **vert)
 {
     info->left_edge_func = (void (*)()) gri_scale_edge;
     info->right_edge_func = (void (*)()) gr_null;
     info->bm.hlog = 0;
     info->bm.bits = info->clut;
     info->loop_func = (void (*)()) gri_poly_loop;
     info->d = ((uint8_t *) ((int32_t) grd_canvas->bm.row * (int32_t) info->y));
     info->d += (int32_t)grd_canvas->bm.bits;
  return(0);
 }

// ========================================================================
// transparent solid polygon scaler
int32_t gri_trans_solid_scale_umap_init(grs_tmap_loop_info *tli, grs_vertex **vert)
 {
    tli->bm.hlog=GRL_TRANS|GRL_SOLID;
    tli->loop_func=(void (*)()) gri_scale_umap_loop_C;
    tli->right_edge_func=gr_null;
    tli->left_edge_func=(void (*)()) gri_scale_edge;
     return(0);
 }

// ========================================================================
// transparent bitmap scaler
int32_t gri_trans_scale_umap_init(grs_tmap_loop_info *tli, grs_vertex **vert)
 {
    tli->bm.hlog=GRL_TRANS;
    tli->loop_func=(void (*)()) gri_scale_umap_loop_C;
    tli->right_edge_func=gr_null;
    tli->left_edge_func=(void (*)()) gri_scale_edge;
     return(0);
 }

// ========================================================================
// opaque bitmap scaler
int32_t gri_opaque_scale_umap_init(grs_tmap_loop_info *tli)
 {
    tli->bm.hlog=GRL_OPAQUE;
    tli->loop_func=(void (*)()) gri_scale_umap_loop_C;
    tli->right_edge_func=gr_null;
    tli->left_edge_func=(void (*)()) gri_scale_edge;
     return(0);
 }

// ========================================================================
// transparent clut bitmap scaler
int32_t gri_trans_clut_scale_umap_init(grs_tmap_loop_info *tli)
 {
    tli->bm.hlog=GRL_TRANS|GRL_CLUT;
    tli->loop_func=(void (*)()) gri_scale_umap_loop_C;
    tli->right_edge_func=gr_null;
    tli->left_edge_func=(void (*)()) gri_scale_edge;
     return(0);
 }

// ========================================================================
// opaque clut bitmap scaler
int32_t gri_opaque_clut_scale_umap_init(grs_tmap_loop_info *tli)
 {
    tli->bm.hlog=GRL_OPAQUE|GRL_CLUT;
    tli->loop_func=(void (*)()) gri_scale_umap_loop_C;
    tli->right_edge_func=gr_null;
    tli->left_edge_func=(void (*)()) gri_scale_edge;
     return(0);
 }

// ========================================================================
// main inside loop for PPC scalers
int32_t gri_scale_umap_loop_C(grs_tmap_loop_info *tli) {
    fix u,ul,du;
    int32_t x;
    uint8_t k;
    fix xl,xr,dx,d;
    uint8_t *p_src,*p_dest;

    xl=fix_cint(tli->left.x);
    xr=fix_cint(tli->right.x);
    if (xr<=xl) return true;
    ul=tli->left.u;
    dx=tli->right.x-tli->left.x;
    du=fix_div(tli->right.u-ul,dx);
    d =fix_ceil(tli->left.x)-tli->left.x;
    ul+=fix_mul(du,d);

    do {
        p_src=tli->bm.bits+tli->bm.row*fix_int(tli->left.v);
        p_dest = grd_bm.bits + (grd_bm.row*tli->y) + xl;
        switch (tli->bm.hlog) {
        case GRL_OPAQUE:
            for (x=xl,u=ul; x<xr; x++) {
                *(p_dest++) = p_src[fix_fint(u)];        // gr_fill_upixel(k,x,tli->y);
                u+=du;
            }
            break;
        case GRL_TRANS:
            for (x=xl,u=ul; x<xr; x++) {
                if ((k=p_src[fix_fint(u)]) != 0) *p_dest = k;        // gr_fill_upixel(k,x,tli->y);
                u+=du;
                p_dest++;
            }
            break;
        case GRL_OPAQUE|GRL_CLUT:
            for (x=xl,u=ul; x<xr; x++) {
                *(p_dest++) = tli->clut[p_src[fix_fint(u)]];    // gr_fill_upixel(tli->clut[k],x,tli->y);
                u+=du;
            }
            break;
        case GRL_TRANS|GRL_CLUT:
            for (x=xl,u=ul; x<xr; x++) {
                if ((k=p_src[fix_fint(u)]) != 0) *p_dest = tli->clut[k];    // gr_fill_upixel(tli->clut[k],x,tli->y);
                u+=du;
                p_dest++;
            }
            break;
        case GRL_TRANS|GRL_SOLID:
            for (x=xl,u=ul; x<xr; x++) {
                if ((k=p_src[fix_fint(u)]) != 0) *p_dest = (uint8_t )(tli->clut);    // gr_fill_upixel((uint8_t )(tli->clut),x,tli->y);
                u+=du;
                p_dest++;
            }
            break;
        }
        tli->left.v+=tli->left.dv;
        tli->y++;
    } while (--(tli->n) > 0);

    return false; /* tmap OK */
}
