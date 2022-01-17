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
 * $Source: n:/project/lib/src/2d/RCS/pixfill.c $
 * $Revision: 1.1 $
 * $Author: lmfeeney $
 * $Date: 1994/06/11 00:57:49 $
*/

#include "grrend.h"
#include "grpix.h"
#include "pixtab.h"

/* not all pixel fill routines use all parameters */
// MLA #pragma off (unreferenced)

// gri_set_fill_globals was inline in 2D.h, moved to here
void gri_set_fill_globals(int32_t *fill_type_ptr, int32_t fill_type,
                                  void (***function_table_ptr)(), void (**function_table)(),
                                  grt_uline_fill **line_vector_ptr, grt_uline_fill *line_vector);

void gri_set_fill_globals(int32_t *fill_type_ptr, int32_t fill_type,
                                  void (***function_table_ptr)(), void (**function_table)(),
                                  grt_uline_fill **line_vector_ptr, grt_uline_fill *line_vector)
 {
    *fill_type_ptr = fill_type;
    *function_table_ptr = function_table;
    *line_vector_ptr = line_vector;
 }


void
gri_pixel_ns (int32_t c, int32_t parm, int32_t x, int32_t y)
{
  /* normal and solid -- do nothing */
  gr_set_upixel(c,x,y);
}

void
gri_pixel_clut (int32_t c, int32_t parm, int32_t x, int32_t y)
{
  /* lookup in clut */
  c = (int32_t) (((uint8_t*)parm)[c]);
  gr_set_upixel(c,x,y);
}

void
gri_pixel_xor (int32_t c, int32_t parm, int32_t x, int32_t y)
{
  /* xor with what's already there */
  c = c ^ gr_get_upixel(x,y);
  gr_set_upixel(c,x,y);
}

void
gri_pixel_blend (int32_t c, int32_t parm, int32_t x, int32_t y)
{
  /* lots to do */
  gr_set_upixel(c,x,y);
}


