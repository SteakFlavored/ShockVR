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
//#define Malloc malloc

#include <string.h>
//#include <malloc.h>
//#include <mprintf.h>
#include <texttool.h>

#pragma require_prototypes off

// monochrome font package
int32_t mono_c_wid(int8_t c)
 { return 1; }
int32_t mono_s_wid(int8_t *s)
 { return strlen(s); }
int32_t mono_c_draw(int8_t c, int32_t xpos, int32_t ypos)
 { mput(c,xpos,ypos); return 1; }
//int mono_s_draw(int8_t *s, int32_t xpos, int32_t ypos)
// { mono_setxy(xpos,ypos); mprint(s); return strlen(s); }
//int anal_s_draw(int8_t *s, int32_t xpos, int32_t ypos)
// { int32_t ox,oy; mono_getxy(&ox,&oy); mono_setxy(xpos,ypos); mprint(s); mono_setxy(ox,oy); return strlen(s); }
int32_t mono_l_clr(int32_t len, int32_t xpos, int32_t ypos)
 { mono_setxy(xpos,ypos); for (; len>0; len--) mprint(" "); return len; }
int32_t mono_s_clr(int8_t *s, int32_t xpos, int32_t ypos)
 { mono_l_clr(strlen(s),xpos,ypos); return strlen(s);}
int32_t mono_c_clr(int8_t c, int32_t xpos, int32_t ypos)
 { mono_l_clr(1,xpos,ypos); return 1; }
int32_t mono_cursor(int32_t x, int32_t y)
 { return mono_setxy(x,y); }

#ifdef nOAF
TTFontInfo *build_mono_TTFI(TextToolFontInfo *mono_font)
{
    if (mono_font==NULL) mono_font=(TextToolFontInfo *)Malloc(sizeof(TextToolFontInfo));
    mono_font->s_draw=mono_s_draw; mono_font->s_wid=mono_s_wid;
    mono_font->c_draw=mono_c_draw; mono_font->c_wid=mono_c_wid;
    mono_font->c_clr=mono_c_clr; mono_font->s_clr=mono_s_clr; mono_font->l_clr=mono_l_clr;
    mono_font->height=1; mono_font->base_w=1; mono_font->type=TTFI_FIXED;
    return mono_font;
}
#endif
