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
#include "frprotox.h"

#ifdef SVGA_SUPPORT
extern void ss_string(int8_t *s, int16_t x, int16_t y);
extern void ss_bitmap(grs_bitmap *bmp, int16_t x, int16_t y);
extern void ss_ubitmap(grs_bitmap *bmp, int16_t x, int16_t y);
extern void ss_scale_bitmap(grs_bitmap *bmp, int16_t x, int16_t y, int16_t w, int16_t h);
extern void ss_noscale_bitmap(grs_bitmap *bmp, int16_t x, int16_t y);
extern void ss_rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
extern void ss_box(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
extern void ss_int_line(int16_t x1, int16_t y1,int16_t x2,int16_t y2);
extern void ss_thick_int_line(int16_t x1, int16_t y1,int16_t x2,int16_t y2);
extern void ss_int_disk(int16_t x1, int16_t y1,int16_t rad);
extern void ss_safe_set_cliprect(int16_t x1,int16_t y1,int16_t x2,int16_t y2);
extern void ss_cset_cliprect(grs_canvas *pcanv, int16_t x1,int16_t y1,int16_t x2,int16_t y2);
extern void ss_vline(int16_t x1,int16_t y1,int16_t y2);
extern void ss_hline(int16_t x1,int16_t y1,int16_t y2);
extern void ss_fix_line(fix x1, fix y1, fix x2, fix y2);
extern void ss_thick_fix_line(fix x1, fix y1, fix x2, fix y2);
extern void ss_get_bitmap(grs_bitmap *bmp, int16_t x, int16_t y);
extern void ss_set_pixel(int32_t color, int16_t x, int16_t y);
extern void ss_set_thick_pixel(int32_t color, int16_t x, int16_t y);
extern void ss_clut_ubitmap(grs_bitmap *bmp, int16_t x, int16_t y, uint8_t *cl);
extern void ss_recompute_zoom(frc *w, int16_t oldm);
extern void ss_mouse_convert(int16_t *px, int16_t *py, bool down);
extern void ss_mouse_convert_round(int16_t *px, int16_t *py, bool down);
extern void ss_point_convert(int16_t *px, int16_t *py, bool down);

extern void gr2ss_register_init(int8_t convert_type, int16_t init_x, int16_t init_y);
extern void gr2ss_register_mode(int8_t conv_mode, int16_t nx, int16_t ny);

extern int16_t ss_curr_mode_width(void);
extern int16_t ss_curr_mode_height(void);
extern void ss_set_hack_mode(int16_t new_m, int16_t *tval);

#define MAX_CONVERT_TYPES  4
#define MAX_USE_MODES      8

extern fix convert_x[MAX_CONVERT_TYPES][MAX_USE_MODES];
extern fix convert_y[MAX_CONVERT_TYPES][MAX_USE_MODES];
extern fix inv_convert_x[MAX_CONVERT_TYPES][MAX_USE_MODES];
extern fix inv_convert_y[MAX_CONVERT_TYPES][MAX_USE_MODES];

extern int8_t convert_type;
extern int8_t convert_use_mode;

extern int16_t MODE_SCONV_X(int16_t cval, int16_t m);
extern int16_t MODE_SCONV_Y(int16_t cval, int16_t m);

#define SCONV_X(x)   fast_fix_mul_int(fix_make((x),0), convert_x[convert_type][convert_use_mode])
#define SCONV_Y(y)   fast_fix_mul_int(fix_make((y),0), convert_y[convert_type][convert_use_mode])
#define RSCONV_X(x)   fix_int(0x8000 + fast_fix_mul(fix_make((x),0), convert_x[convert_type][convert_use_mode]))
#define RSCONV_Y(y)   fix_int(0x8000 + fast_fix_mul(fix_make((y),0), convert_y[convert_type][convert_use_mode]))

#define INV_SCONV_X(x)   fast_fix_mul_int(fix_make((x),0), inv_convert_x[convert_type][convert_use_mode])
#define INV_SCONV_Y(y)   fast_fix_mul_int(fix_make((y),0), inv_convert_y[convert_type][convert_use_mode])

#define FIXCONV_X(x)   fast_fix_mul((x), convert_x[convert_type][convert_use_mode])
#define FIXCONV_Y(y)   fast_fix_mul((y), convert_y[convert_type][convert_use_mode])
#define INV_FIXCONV_X(x)   fast_fix_mul((x), inv_convert_x[convert_type][convert_use_mode])
#define INV_FIXCONV_Y(y)   fast_fix_mul((y), inv_convert_y[convert_type][convert_use_mode])

extern uint8_t gr2ss_override;

#define OVERRIDE_NONE   0x00
#define OVERRIDE_SCALE  0x01
#define OVERRIDE_FONT   0x02
#define OVERRIDE_CLIP   0x04
#define OVERRIDE_GET_BM 0x10
#define OVERRIDE_ALL    0x7F
#define OVERRIDE_FAIL   0x80


#else

#define ss_string(s,x,y) gr_string(s,x,y)
#define ss_bitmap(bmp,x,y) gr_bitmap(bmp,x,y)
#define ss_ubitmap(bmp,x,y) gr_ubitmap(bmp,x,y)
#define ss_noscale_bitmap(bmp,x,y) gr_bitmap(bmp,x,y)
#define ss_scale_bitmap(bmp,x,y,w,h) gr_scale_bitmap(bmp,x,y,w,h)
#define ss_rect(x1,y1,x2,y2) gr_rect(x1,y1,x2,y2)
#define ss_box(x1,y1,x2,y2) gr_box(x1,y1,x2,y2)
#define ss_int_line(x1,y1,x2,y2) gr_int_line(x1,y1,x2,y2)
#define ss_thick_int_line(x1,y1,x2,y2) gr_int_line(x1,y1,x2,y2)
#define ss_int_disk(x1,y1,rad) gr_int_disk(x1,y1,rad)
#define ss_safe_set_cliprect(x1,y1,x2,y2) gr_safe_set_cliprect(x1,y1,x2,y2)
#define ss_cset_cliprect(pcanv,x1,y1,x2,y2) gr_cset_cliprect(pcanv, x1,y1,x2,y2)
#define ss_vline(x1,y1,y2) gr_vline(x1,y1,y2)
#define ss_hline(x1,y1,y2) gr_hline(x1,y1,y2)
#define ss_fix_line(x1,y1,x2,y2) gr_fix_line(x1,y1,x2,y2)
#define ss_thick_fix_line(x1,y1,x2,y2) gr_fix_line(x1,y1,x2,y2)
#define ss_get_bitmap(bmp,x,y) gr_get_bitmap(bmp,x,y)
#define ss_set_pixel(color,x,y) gr_set_pixel(color,x,y)
#define ss_set_thick_pixel(color,x,y) gr_set_pixel(color,x,y)
#define ss_clut_ubitmap(bmp,x,y,cl) gr_clut_ubitmap(bmp,x,y,cl)
#define ss_recompute_zoom(w,oldm)

#define gr2ss_register_init(convert_type,init_x,init_y)
#define gr2ss_register_mode(conv_mode,nx,ny)

extern void ss_mouse_convert(int16_t *px, int16_t *py, bool down);
extern void ss_mouse_convert_round(int16_t *px, int16_t *py, bool down);

#define SCONV_X(x)   x
#define SCONV_Y(y)   y
#define INV_SCONV_X(x)   x
#define INV_SCONV_Y(y)   y

#define FIXCONV_X(x)   x
#define FIXCONV_Y(y)   y
#define INV_FIXCONV_X(x)   x
#define INV_FIXCONV_Y(y)   y

#define MODE_SCONV_X(x,m)  x
#define MODE_SCONV_Y(y,m)  y
#endif

