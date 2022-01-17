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
 * $Source: r:/prj/cit/src/inc/RCS/frparams.h $
 * $Revision: 1.3 $
 * $Author: dc $
 * $Date: 1994/07/20 23:05:51 $
 *
 * Citadel Renderer
 *  global parameters structures, setting, and defines
 *
 * $Log: frparams.h $
 * Revision 1.3  1994/07/20  23:05:51  dc
 * odrop
 *
 * Revision 1.2  1994/01/30  01:54:53  dc
 * global lighting
 *
 * Revision 1.1  1994/01/02  17:16:32  dc
 * Initial revision
 *
 */

#include "frctxt.h"

#define TM_SIZE_CNT 3                  /* # of different tmap sizes */

typedef struct {
   struct {
      uint8_t main:1;                    /* any textures? */
      uint8_t wall:1;                    /*  if any, any walls */
      uint8_t floor:1;                   /*          any floor */
      uint8_t ceiling:1;                 /*          any ceiling */
      uint8_t cyber:1;                   /* cyberspace instead */
      uint8_t cyber_full:1;              /* full cursors */
   } faces;
   struct {
      uint8_t highlights:1;              /* tilemap highlights of cones */
      uint8_t tilecursor:1;              /* show the tilemap cursor in 3d */
      uint8_t nodraw:1;                  /* dont actually render FB */
   } features;
   struct {
#ifdef C_WERE_SUPER_COOL
      uint8_t lighting:1;                /* any lighting at all? */
      uint8_t terrain:1;                 /* terrain light values checked and used? */
      uint8_t camera:1;                  /* camera light values used */
      uint8_t normal_chk:1;              /* use normal when computing wall lighting */
#else
      uint8_t flags;                     /* 0,0,0,0,any,terr,cam,normal */
#endif
      int32_t   normal_shf;                /* what to shift normal by when adding to dist */
      uint8_t rad[2];                    /* inner, outer lighting radius */
      uint8_t base[2];                   /* inner, outer light values */
      fix   slope;                     /* slope and yintercept */
      fix   yint;                      /*  of lighting line */
      int16_t global_mod;                /* change to all lighting */
   } lighting;
   struct {
      uint8_t qscale_obj;                /* radius at which to qscale objects */
      uint8_t qscale_crit;               /* radius at which to qscale critters */
      uint8_t qscale_texture;            /* radius at which to qscale textures */
      uint8_t detail;                    /* detail setting */
      uint8_t clear_color;               /* color to clear background too */
      uint8_t drop_rad[TM_SIZE_CNT];     /* radii to switch to lower res tmaps */
      uint8_t odrop_rad[TM_SIZE_CNT];    /* original (base) drop radii to switch to lower res tmaps */
      uint8_t radius;                    /* maximal view radius */
      uint8_t show_all:1;                /* render whole world - no clip */
      uint8_t cone_only:1;               /* no 2 1/2D clip, cone only */
   } view;
   struct {
      int32_t  last_chk_time;
      int32_t  last_frame_cnt;
      int32_t  tot_frame_cnt;
      int32_t  last_frame_len;
   } time;
} fauxrend_parameters;

#ifndef __FRSETUP_SRC
extern fauxrend_parameters _frp;
#define get_frp() (_frp)
#endif

#define LIGHT_BITS_MASK 0xf
#define LIGHT_BITS_ANY  0x8
#define LIGHT_BITS_TERR 0x4
#define LIGHT_BITS_CAM  0x2
#define LIGHT_BITS_NORM 0x1
#define LIGHT_BITS_HOW (LIGHT_BITS_NORM|LIGHT_BITS_CAM|LIGHT_BITS_TERR)

#define _frp_light_bits_any()  (_frp.lighting.flags&LIGHT_BITS_ANY)
#define _frp_light_bits_how()  (_frp.lighting.flags&LIGHT_BITS_HOW)
#define _frp_light_bits_cam()  (_frp.lighting.flags&LIGHT_BITS_CAM)
#define _frp_light_bits_norm() (_frp.lighting.flags&LIGHT_BITS_NORM)
#define _frp_light_bits_terr() (_frp.lighting.flags&LIGHT_BITS_TERR)

#define _frp_light_bits_set(m)   (_frp.lighting.flags|=m)
#define _frp_light_bits_clear(m) (_frp.lighting.flags&=~m)
#define _frp_light_bits_tog(m)   if (_frp.lighting.flags|m) _frp_light_bits_clear(m) else _frp_light_bits_set(m)
