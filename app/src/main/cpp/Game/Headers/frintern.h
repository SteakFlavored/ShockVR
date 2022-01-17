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
 * $Source: n:/project/cit/src/inc/RCS/frintern.h $
 * $Revision: 1.3 $
 * $Author: dc $
 * $Date: 1994/01/16 05:22:06 $
 *
 * Citadel Renderer
 *  internal prototypes for the renderer
 *
 * $Log: frintern.h $
 * Revision 1.3  1994/01/16  05:22:06  dc
 * facelet parse obj
 *
 * Revision 1.2  1994/01/12  22:04:22  dc
 * facelet code hacking
 *
 * Revision 1.1  1994/01/02  17:16:27  dc
 * Initial revision
 *
 */
#ifndef __FRINTERN_H
#define __FRINTERN_H

#include "frcamera.h"
#include "frprotox.h"
#include "frshipm.h"
#include "frworld.h"

//======== From frsetup.c
// setup current view, send it out
int32_t      fr_prepare_view(frc *view);
int32_t      fr_start_view (void);
int32_t      fr_send_view (void);

#ifndef __FRSETUP_SRC
#ifdef __FRTYPES_H
extern fauxrend_context *_fr, *_sr;
#endif // only know about context itself if you already include types
extern uint32_t _fr_curflags, _fr_glob_flags;
extern uint8_t *_fr_clut_list[4];
#endif // __FRSETUP_SRC

//======== From frpipe.c
// pipe setup and control
int32_t      fr_pipe_resize(int32_t x, int32_t y, int32_t z, void *mptr);
int32_t      fr_pipe_start(int32_t rad);
int32_t      fr_pipe_go(void);
int32_t      fr_pipe_end(void);
int32_t      fr_pipe_freemem(void);

#ifndef __FRPIPE_SRC
extern int32_t fr_map_x,fr_map_y,fr_map_z;
extern int32_t _fr_x_cen, _fr_y_cen;
#endif // __FRPIPE_SRC

//======== from frpts.c
int32_t      fr_pts_frame_start(void);
int32_t      fr_pts_resize(int32_t x, int32_t y);
int32_t      fr_pts_freemem(void);
int32_t      fr_pts_update(int32_t y, int32_t lx, int32_t rx);
int32_t      fr_pts_setup(int32_t pt_code);                // must call before update

#ifndef __FRPTS_SRC
#ifdef __3D_H
extern g3s_phandle *_fr_ptbase, *_fr_ptnext;
#endif // __3D_H
#endif // __FRPTS_SRC

//======== From frclip.c
// these all set and modify global clipping arrays
// so, we cannot do these in parallel
int32_t      fr_clip_resize(int32_t x,int32_t y);
int32_t      fr_clip_frame_start(void);
int32_t      fr_clip_frame_end(void);
int32_t      fr_clip_cone(void);
int32_t      fr_clip_tile(void);
int32_t      fr_clip_freemem(void);

//======== From frtables.c
// setup and integrity test various renderer data tables
int32_t      fr_tables_build(void);

//======== From frobj.c
void     render_parse_obj(void);
void     facelet_parse_obj(void);

//======== From frutil.c
#define FR_CUR_OBJ_BASE 65
#ifndef __FRUTIL_SRC
extern uint8_t  fr_cur_obj_col;
extern uint16_t fr_col_to_obj[256];
#endif

//======== From frterr.c
void     fr_draw_tile(void);
void     fr_terr_frame_start(void);
void     fr_terr_frame_end(void);
void    _fr_facelet_init(void);
#ifdef __3D_H
int32_t     _fr_do_light(g3s_phandle work, int32_t hgt_code);
#endif

#ifndef __FRTERR_SRC
// map/world data layout
#ifdef __3D_H
extern sfix _fr_sfuv_list[];
#endif
extern fix _fr_fhgt_step;
extern fix _fr_fhgt_list[];
extern fix slope_norm[][3];
extern int32_t wall_adds[], csp_trans_add[];
// pipeline controller
extern int32_t _fdt_x, _fdt_y, _fdt_mask, _fdt_dist, _fdt_pbase;
#ifdef __MAP_H
extern MapElem *_fdt_mptr;
#endif
#endif

#endif // __FRINTERN_H

// she flew low above the highway
// and i saw the wind
// throwing back her barbie doll hair
// robin flies again, robin flies again

// and in a kitchen in kentucky
// she thinks she's peter pan
// and in the bottom of her concrete basement
// robin flies again, robin flies again
