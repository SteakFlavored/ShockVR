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
 * $Source: r:/prj/lib/src/2d/RCS/fl8p.h $
 * $Revision: 1.11 $
 * $Author: kevin $
 * $Date: 1994/11/02 19:39:49 $
 *
 * Miscellaneous stuff needed by the flat8 perspective mappers.
 * Includes 3 and 12 bit integer fixed point macros.
 *
 * This file is part of the 2d library.
 *
 * $Log: fl8p.h $
 * Revision 1.11  1994/11/02  19:39:49  kevin
 * Use 20 bit integer fixed point for bitmap coord intermediates to avoid overflows in svga.
 *
 * Revision 1.10  1994/07/18  17:04:27  kevin
 * removed unnecessary includes.
 *
 * Revision 1.9  1994/01/16  12:04:44  kevin
 * Added clut lighting tolerance global declaration.
 *
 * Revision 1.8  1994/01/03  22:26:11  kevin
 * Added declaration for global grd_per_setup.
 *
 * Revision 1.7  1993/12/17  01:00:57  kevin
 * Added egregious error constants.
 *
 * Revision 1.6  1993/12/15  02:49:06  kevin
 * Added wlog and hlog to context.
 *
 * Revision 1.5  1993/12/14  22:31:14  kevin
 * Moved declaration of grd_per_context to
 * grpm.h so everyone can use it.
 *
 * Revision 1.4  1993/12/08  23:44:52  kevin
 * Added vtab to per_setup structure for non/power/of/2 support.
 *
 * Revision 1.3  1993/12/04  17:28:28  kevin
 * Added clut field to per_setup structure.
 *
 * Revision 1.2  1993/12/04  12:33:01  kevin
 * Added new structures, new fixed point primitives.
 *
 * Revision 1.1  1993/11/18  23:42:36  kevin
 * Initial revision
 *
*/

#ifndef __FL8P_H
#define __FL8P_H

#define fix_mul_div_3_16_16_3 fix_mul_div
#define fix_mul_div_3_16_3_16 fix_mul_div
#define fix_mul_div_3_8_8_3 fix_mul_div

// MLA- moved implementations of these into Permap.c, since we can't do inline asm
extern fix fix_div_16_16_3 (fix a, fix b);
extern fix fix_mul_3_3_3 (fix a, fix b);
extern fix fix_mul_3_32_16 (fix a, fix b);
extern fix fix_mul_3_16_20 (fix a, fix b);
extern fix fix_mul_16_32_20 (fix a, fix b);

#define fix_div_16_3_16 fix_div_16_16_3
#define fix_div_3_3_16 fix_div
#define fix_div_3_16_3 fix_div
#define fix_div_3_8_3 fix_div_16_8_16
#define fix_div_8_8_8 fix_div_16_8_16
#define fix_mul_3_16_16 fix_mul_3_3_3
#define fix_mul_3_8_8 fix_mul_3_3_3

#define fix_sal(a,b) ((a)<<(b))
#define fix_sar(a,b) ((a)>>(b))

#define fix_3_12(a) ((a)>>9)
#define fix_12_16(a) ((a)>>4)
#define fix_3_16(a) ((a)>>13)
#define fix_3_8(a) ((a)>>5)

#define FIX_UNIT_3 0x20000000

#define ACENT (grd_bm.w>>1)
#define BCENT (grd_bm.h>>1)
#define SCALE 0x200

#define GR_PER_CODE_OK 0
#define GR_PER_CODE_MEMERR 1
#define GR_PER_CODE_BADPLANE 2
#define GR_PER_CODE_BADDENOM 3
#define GR_PER_CODE_BADINDEX 4
#define GR_PER_CODE_LIN 5
#define GR_PER_CODE_WALL 6
#define GR_PER_CODE_FLOOR 7
#define GR_PER_CODE_BIGSLOPE 8
#define GR_PER_CODE_SMALLSLOPE 9

#define HS_TS_ERR 10
#define HS_TB_ERR 11
#define VS_TS_ERR 12
#define VS_TB_ERR 13

extern uint8_t flat8_per_ltol;
extern uint8_t flat8_per_wftol;
extern fix gr_clut_lit_tol;

#endif
