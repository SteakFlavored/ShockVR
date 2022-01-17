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
#ifndef __FAKEFONT_H
#define __FAKEFONT_H

// prototypes
// drawing - return the width drawn
int32_t fnt_c_draw(int8_t  c, int32_t xpos, int32_t ypos);
int32_t fnt_s_draw(int8_t *s, int32_t xpos, int32_t ypos);
int32_t fnt_c_width(int8_t  c);
int32_t fnt_s_width(int8_t *s);
int32_t fnt_l_clr(int32_t len, int32_t xpos, int32_t ypos);
int32_t fnt_s_clr(int8_t *s, int32_t xpos, int32_t ypos);
int32_t fnt_c_clr(int8_t c, int32_t xpos, int32_t ypos);
int32_t fnt_no_cursor(int32_t x, int32_t y);

// load/system
int32_t fnt_init(int8_t *def_fname);        // ret handle of def_fname, -1 fail,
                                                //  0 if def_fname NULL + succesful
int32_t fnt_init_from_style(void *style_ptr);
bool fnt_free(void);                     // free the font system
int32_t fnt_load(int8_t *fnt_fname);        // returns the handle
int32_t fnt_load_from_style(void *style_ptr);
bool fnt_unload(int32_t fnt_handle);     // these both
bool fnt_select(int32_t fnt_handle);     //    return success or not

// for texttools...
#ifdef NOT
TTFontInfo *build_font_TTFI(TextToolFontInfo *tfont);
#endif

// contstants
#define FNT_MAX_CHARS 128

#endif
