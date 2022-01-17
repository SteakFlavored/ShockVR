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
#ifndef __FRPROTOX_H
#define __FRPROTOX_H
/*
 * $Source: r:/prj/cit/src/inc/RCS/frprotox.h $
 * $Revision: 1.11 $
 * $Author: xemu $
 * $Date: 1994/08/05 03:06:45 $
 *
 * Citadel Renderer
 *  global external prototypes for the renderer
 *
 * $Log: frprotox.h $
 * Revision 1.11  1994/08/05  03:06:45  xemu
 * look, fr_get_at with transparency
 *
 * Revision 1.10  1994/04/23  09:56:24  xemu
 * new params1
 *
 * Revision 1.9  1994/04/14  15:00:33  kevin
 * New detail stuff.
 *
 * Revision 1.8  1994/03/13  17:18:33  dc
 * more fields for obj_block
 *
 * Revision 1.7  1994/03/03  12:18:27  dc
 * place view takes a canvas now
 *
 * Revision 1.6  1994/02/13  05:48:00  dc
 * rend_start
 *
 * Revision 1.5  1994/01/02  17:16:34  dc
 * Initial revision
 *
 */

#ifndef __FRTYPESX_H
typedef void frc;
typedef void fmp;
#endif

//======== Basic truths
#define FR_OK         (1)
#define FR_BAD_VIEW  (-1)
#define FR_NOMEM     (-2)
#define FR_NULL_PTR  (-3)
#define FR_NO_NEED   (-4)

//======== Random prettiness
#define FR_NOCAM     ((void *)(-1))
#define FR_DEFCAM    (NULL)
#define FR_DEFVIEW   (NULL)
#define FR_NEWVIEW   (NULL)

//======== From frsetup.c
// global initialization
void fr_startup (void);
void fr_shutdown(void);
void fr_closedown(void);

// view control/setup
frc    *fr_place_view (frc *view, void *cam, void *canvas, int32_t pflags, int8_t axis, int32_t fov, int32_t xc, int32_t yc, int32_t wid, int32_t hgt);
void    fr_use_global_detail (frc *view);
int32_t     fr_view_resize(frc *view, int32_t wid, int32_t hgt);
int32_t     fr_view_full(frc *view, int32_t wid, int32_t hgt);
int32_t     fr_mod_size (frc *view, int32_t xc, int32_t yc, int32_t wid, int32_t hgt);
int32_t     fr_mod_cams (frc *view, void *cam, int32_t mod_fac);
int32_t     fr_context_mod_flag (frc *view, int32_t pflags_on, int32_t pflags_off);   // remember to set flags_off for things you turn on
int32_t     fr_global_mod_flag  (int32_t flags_on, int32_t flags_off);
void   *fr_get_canvas (frc *view);                                // really returns a grs_canvas, but no want 2d.h
int32_t     fr_set_view (frc *view);
int32_t     fr_free_view (frc *view);
void    fr_set_cluts(uint8_t *base, uint8_t *bwclut, uint8_t *greenclut, uint8_t *amberclut);
int32_t    fr_set_callbacks(frc *view,
								int32_t (*draw)(void *dstc, void *dstbm, int32_t x, int32_t y, int32_t flg),
                     				void (*horizon)(void *dstbm, int32_t flg),
                     				void (*render)(void *dstbm, int32_t flg));
int32_t    fr_set_global_callbacks( int32_t (*draw)(void *dstc, void *dstbm, int32_t x, int32_t y, int32_t flg),
                     							void (*horizon)(void *dstbm, int32_t flg),
              								void (*render)(void *dstbm, int32_t flg) );

//======== From frcompil.c
void    fr_compile_rect(fmp *fm, int32_t llx, int32_t lly, int32_t ulx, int32_t uly, bool seen_bits);
void    fr_compile_restart(fmp *fm);

//======== From frmain.c
int32_t     fr_rend(frc *view);
uint16_t  fr_get_at(frc *view, int32_t x, int32_t y,bool transp);

//======== From frutil.c
int8_t   *fr_get_frame_rate(void);


//======== Externals to provide, initialized to dumb things
#ifndef __FRSETUP_SRC
extern int32_t   _fr_default_detail;
extern int32_t   _fr_global_detail;
extern void   (*fr_mouse_hide)(void), (*fr_mouse_show)(void);
extern int32_t   (*fr_get_idx)(void);
extern bool  (*fr_obj_block)(void *mptr, uint8_t *_sclip, int32_t *loc);
extern void  (*fr_clip_start)(bool headnorth);
extern void  (*fr_rend_start)(void);
#ifdef __2D_H
extern grs_bitmap *(*fr_get_tmap)(void);
#endif
#endif

// default versions of above, defined in frsetup and set there
void fr_default_mouse(void);
int32_t   fr_default_idx(void), fr_pickup_idx(void);
bool  fr_default_block(void *mptr, uint8_t *_sclip, int32_t *loc);
void  fr_default_clip_start(bool headnorth);
void  fr_default_rend_start(void);
#ifdef __2D_H
grs_bitmap *fr_default_tmap(void);
#endif

#endif // __FRPROTOX_H
