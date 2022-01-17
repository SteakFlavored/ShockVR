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
 * $Source: r:/prj/lib/src/2d/RCS/temptm.c $
 * $Revision: 1.9 $
 * $Author: kevin $
 * $Date: 1994/11/09 21:14:00 $
 *
 * Temporary texture mapper dispatchers.
 *
 * This file is part of the 2d library.
 *
 */

#include "bitmap.h"
#include "clpcon.h"
#include "cnvdat.h"
#include "fill.h"
#include "grpix.h"
#include "grs.h"
#include "plytyp.h"
#include "ifcn.h"
#include "tabdat.h"
#include "tmapfcn.h"
#include "tmaps.h"
#include "general.h"

#include "grs.h"

enum {
   POLY, SPOLY, CPOLY, TPOLY, STPOLY
};

// prototypes
void temp_upoint(int16_t x, int16_t y);
void temp_point(int16_t x, int16_t y);
void temp_flat8_mask_bitmap (grs_bitmap *bm, int32_t x, int32_t y, grs_stencil *sten);
void temp_flat8_clut_ubitmap (grs_bitmap *bm, int32_t x, int32_t y, uint8_t *cl);
void temp_tluc8_ubitmap (grs_bitmap *bm, int32_t x, int32_t y);
void temp_per_map (grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_clut_per_map (grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *clut);
void temp_clut_per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *clut);
void temp_lit_per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_lin_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_lin_map(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_lit_lin_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_lit_lin_map(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_clut_lin_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl);
void temp_clut_lin_map(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl);
void temp_floor_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_clut_floor_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl);
void temp_lit_floor_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_wall_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_clut_wall_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl);
void temp_lit_wall_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl);
void temp_scale_umap(grs_bitmap *bm, int16_t x, int16_t y, int16_t w, int16_t h);
void temp_clut_scale_umap(grs_bitmap *bm, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *cl);
int32_t gri_scale_clip(grs_vertex *v0, grs_vertex *v1);

void temp_rsd8_ubitmap (grs_bitmap *bm, int32_t x, int32_t y);
void temp_rsd8_bitmap (grs_bitmap *bm, int32_t x, int32_t y);


void temp_upoint(int16_t x, int16_t y)
{
   gr_fill_upixel(grd_gc.fcolor, x, y);
}

void temp_point(int16_t x, int16_t y)
{
   gr_fill_pixel(grd_gc.fcolor, x, y);
}

void temp_rsd8_ubitmap (grs_bitmap *bm, int32_t x, int32_t y)
{
   ((void (*)(grs_bitmap *_bm,int32_t _x, int32_t _y))
      grd_function_table[GRC_BITMAP+BMT_RSD8*GRD_FUNCS])(bm, x, y);
}

void temp_rsd8_bitmap (grs_bitmap *bm, int32_t x, int32_t y)
{
   ((void (*)(grs_bitmap *_bm,int32_t _x, int32_t _y, grs_stencil *_sten))
      grd_function_table[GRC_STENCIL_BITMAP+BMT_RSD8*GRD_FUNCS])(bm, x, y, grd_clip.sten);
}

void temp_flat8_mask_bitmap (grs_bitmap *bm, int32_t x, int32_t y, grs_stencil *sten)
{
   ((void (*)(grs_bitmap *_bm,int32_t _x, int32_t _y, grs_stencil *_sten))
      grd_function_table[GRC_STENCIL_BITMAP+BMT_FLAT8*GRD_FUNCS])(bm, x, y, sten);
}

void temp_flat8_clut_ubitmap (grs_bitmap *bm, int32_t x, int32_t y, uint8_t *cl)
{
   ((void (*)(grs_bitmap *_bm,int32_t _x, int32_t _y, uint8_t *_cl))
      grd_function_table[GRC_BITMAP+BMT_FLAT8*GRD_FUNCS])(bm, x, y, cl);
}

void temp_tluc8_ubitmap (grs_bitmap *bm, int32_t x, int32_t y)
{
   ((void (*)(grs_bitmap *_bm,int32_t _x, int32_t _y))
      grd_function_table[GRC_BITMAP+BMT_TLUC8*GRD_FUNCS])(bm, x, y);
}

int32_t temp_poly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=POLY;
   bm.flags=0;
   return h_map(&bm,n,vpl,&ti);
}

void temp_upoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=POLY;
   bm.flags=0;
   h_umap(&bm,n,vpl,&ti);
}

int32_t temp_spoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=SPOLY;
   bm.flags=0;
   return h_map(&bm,n,vpl,&ti);
}

void temp_uspoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=SPOLY;
   bm.flags=0;
   h_umap(&bm,n,vpl,&ti);
}

int32_t temp_cpoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=CPOLY;
   bm.flags=0;
   return h_map(&bm,n,vpl,&ti);
}

void temp_ucpoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=CPOLY;
   bm.flags=0;
   h_umap(&bm,n,vpl,&ti);
}

int32_t temp_tpoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   bm.bits=(uint8_t *)c;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=TPOLY;
   bm.flags=0;
   return h_map(&bm,n,vpl,&ti);
}

void temp_utpoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=TPOLY;
   bm.flags=0;
   h_umap(&bm,n,vpl,&ti);
}

int32_t temp_stpoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=STPOLY;
   bm.flags=0;
   return h_map(&bm,n,vpl,&ti);
}

void temp_ustpoly (int32_t c, int32_t n, grs_vertex **vpl)
{
   grs_tmap_info ti;
   grs_bitmap bm;

   ti.tmap_type=GRC_POLY;
   ti.flags=0;
   bm.bits=(uint8_t *)c;
   bm.type=STPOLY;
   bm.flags=0;
   h_umap(&bm,n,vpl,&ti);
}

void temp_per_map (grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_PER;
      ti.flags=0;
      per_map(bm,n,vpl,&ti);
   }
}

void temp_per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_PER;
      ti.flags=0;
      per_umap(bm,n,vpl,&ti);
   }
}

void temp_clut_per_map (grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *clut)
{
  if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_CLUT_PER;
      ti.flags=TMF_CLUT;
      ti.clut=clut;
      per_map(bm,n,vpl,&ti);
   }
}

void temp_clut_per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *clut)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_CLUT_PER;
      ti.flags=TMF_CLUT;
      ti.clut=clut;
      per_umap(bm,n,vpl,&ti);
   }
}

void temp_lit_per_umap (grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_LIT_PER;
      ti.flags=0;
      per_umap(bm,n,vpl,&ti);
   }
}

void temp_lin_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if ((bm->row==1<<(bm->wlog))||(grd_gc.fill_type==FILL_CLUT)) {
      grs_tmap_info ti;
      if ((n==3)&&((bm->flags&BMF_TRANS)==0))
         ti.tmap_type=GRC_LIN;
      else
         ti.tmap_type=GRC_BILIN;
      ti.flags=0;
      h_umap(bm,n,vpl,&ti);
   }
}

void temp_lin_map(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if ((bm->row==1<<(bm->wlog))||(grd_gc.fill_type==FILL_CLUT)) {
      grs_tmap_info ti;
      if ((n==3)&&((bm->flags&BMF_TRANS)==0))
         ti.tmap_type=GRC_LIN;
      else
         ti.tmap_type=GRC_BILIN;
      ti.flags=0;
      h_map(bm,n,vpl,&ti);
   }
}

void temp_lit_lin_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if ((bm->row==1<<(bm->wlog))||(grd_gc.fill_type==FILL_CLUT)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_LIT_BILIN;
      ti.flags=0;
      h_umap(bm,n,vpl,&ti);
   }
}

void temp_lit_lin_map(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if ((bm->row==1<<(bm->wlog))||(grd_gc.fill_type==FILL_CLUT)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_LIT_BILIN;
      ti.flags=0;
      h_map(bm,n,vpl,&ti);
   }
}

void temp_clut_lin_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl)
{
   grs_tmap_info ti;

   if ((n==3)&&((bm->flags&BMF_TRANS)==0))
      ti.tmap_type=GRC_CLUT_LIN;
   else
      ti.tmap_type=GRC_CLUT_BILIN;
   ti.flags=TMF_CLUT;
   ti.clut=cl;
   h_umap(bm,n,vpl,&ti);
}

void temp_clut_lin_map(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl)
{
   grs_tmap_info ti;

   if ((n==3)&&((bm->flags&BMF_TRANS)==0))
      ti.tmap_type=GRC_CLUT_LIN;
   else
      ti.tmap_type=GRC_CLUT_BILIN;
   ti.flags=TMF_CLUT;
   ti.clut=cl;
   h_map(bm,n,vpl,&ti);
}

void temp_floor_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_FLOOR;
      ti.flags=TMF_FLOOR;
      h_umap(bm,n,vpl,&ti);
   }
}

void temp_clut_floor_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_CLUT_FLOOR;
      ti.flags=TMF_CLUT|TMF_FLOOR;
      ti.clut=cl;
      h_umap(bm,n,vpl,&ti);
   }
}

void temp_lit_floor_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_LIT_FLOOR;
      ti.flags=TMF_FLOOR;
      h_umap(bm,n,vpl,&ti);
   }
}

void temp_wall_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_WALL2D;
      ti.flags=TMF_WALL;
      v_umap(bm,n,vpl,&ti);
   }
}

void temp_clut_wall_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl, uint8_t *cl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_CLUT_WALL2D;
      ti.flags=TMF_CLUT|TMF_WALL;
      ti.clut=cl;
      v_umap(bm,n,vpl,&ti);
   }
}

void temp_lit_wall_umap(grs_bitmap *bm, int32_t n, grs_vertex **vpl)
{
   if (bm->row==1<<(bm->wlog)) {
      grs_tmap_info ti;
      ti.tmap_type=GRC_LIT_WALL2D;
      ti.flags=TMF_WALL;
      v_umap(bm,n,vpl,&ti);
   }
}

/* take int32_t _x,_y; fix _u,_v; stuff them into grs_vertex _vertex */

#define make_vertex(_vertex,_x,_y,_u,_v) \
   _vertex.x = fix_make(_x,0), \
   _vertex.y = fix_make(_y,0), \
   _vertex.u = _u,             \
   _vertex.v = _v

void temp_scale_umap(grs_bitmap *bm, int16_t x, int16_t y, int16_t w, int16_t h)
{
   grs_tmap_info ti;
   grs_vertex *vpl[2];
   grs_vertex v0,v1;

   vpl[0]=&v0;
   vpl[1]=&v1;
   make_vertex(v0,x,y,0,0);
   make_vertex(v1,x+w,y+h,fix_make(bm->w,0),fix_make(bm->h,0));

   ti.tmap_type=GRC_SCALE;
   ti.flags=0;
   h_umap(bm,2,vpl,&ti);
}

void temp_clut_scale_umap(grs_bitmap *bm, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *cl)
{
   grs_tmap_info ti;
   grs_vertex *vpl[2];
   grs_vertex v0,v1;

   vpl[0]=&v0;
   vpl[1]=&v1;
   make_vertex(v0,x,y,0,0);
   make_vertex(v1,x+w,y+h,fix_make(bm->w,0),fix_make(bm->h,0));

   ti.tmap_type=GRC_CLUT_SCALE;
   ti.flags=TMF_CLUT;
   ti.clut=cl;
   h_umap(bm,2,vpl,&ti);
}

int32_t gri_scale_clip(grs_vertex *v0, grs_vertex *v1)
{
   int32_t code;
   fix u_scale,v_scale;

   if ((v0->x>=grd_fix_clip.right) || (v1->x<=grd_fix_clip.left) ||
       (v0->y>=grd_fix_clip.bot) || (v1->y<=grd_fix_clip.top))
      return CLIP_ALL;

   code = CLIP_NONE;

   u_scale = fix_div(v1->u,v1->x-v0->x);
   v_scale = fix_div(v1->v,v1->y-v0->y);

   if (v0->x<grd_fix_clip.left) {
      v0->u = fix_mul(u_scale,grd_fix_clip.left-v0->x);
      v0->x = grd_fix_clip.left;
      code |= CLIP_LEFT;
   }
   if (v1->x>grd_fix_clip.right) {
      v1->x = grd_fix_clip.right;
      v1->u = v0->u+fix_mul(u_scale,v1->x-v0->x);
      code |= CLIP_RIGHT;
   }
   if (v0->y<grd_fix_clip.top) {
      v0->v = fix_mul(v_scale,grd_fix_clip.top-v0->y);
      v0->y = grd_fix_clip.top;
      code |= CLIP_TOP;
   }
   if (v1->y>grd_fix_clip.bot) {
      v1->y = grd_fix_clip.bot;
      v1->v = v0->v+fix_mul(v_scale,v1->y-v0->y);
      code |= CLIP_RIGHT;
   }
   return code;
}

int32_t temp_scale_map(grs_bitmap *bm, int16_t x, int16_t y, int16_t w, int16_t h)
{
   grs_tmap_info ti;
   grs_vertex *vpl[2];
   grs_vertex v0,v1;
   int32_t code;

   vpl[0]=&v0;
   vpl[1]=&v1;
   make_vertex(v0,x,y,0,0);
   make_vertex(v1,x+w,y+h,fix_make(bm->w,0),fix_make(bm->h,0));

   code=gri_scale_clip(&v0,&v1);
   if (code==CLIP_ALL) return code;

   ti.tmap_type=GRC_SCALE;
   ti.flags=0;
   h_umap(bm,2,vpl,&ti);
   return code;
}

int32_t temp_clut_scale_map(grs_bitmap *bm, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *cl)
{
   grs_tmap_info ti;
   grs_vertex *vpl[2];
   grs_vertex v0,v1;
   int32_t code;

   vpl[0]=&v0;
   vpl[1]=&v1;
   make_vertex(v0,x,y,0,0);
   make_vertex(v1,x+w,y+h,fix_make(bm->w,0),fix_make(bm->h,0));

   code=gri_scale_clip(&v0,&v1);
   if (code==CLIP_ALL) return code;

   ti.tmap_type=GRC_CLUT_SCALE;
   ti.flags=TMF_CLUT;
   ti.clut=cl;
   h_umap(bm,2,vpl,&ti);
   return CLIP_NONE;
}
