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
 * $Source: r:/prj/lib/src/2d/RCS/fl8wlin.c $
 * $Revision: 1.1 $
 * $Author: kevin $
 * $Date: 1994/08/04 09:53:09 $
 *
 * Routines to draw wire polys.
 *
 * This file is part of the 2d library.
 *
 */

#include "ctxmac.h"
#include "cnvdat.h"
#include "fill.h"
#include "plytyp.h"
#include "linfcn.h"

#define gr_get_ipal_index(r,g,b)    (int32_t) ((((r)>>19) &0x1f) | (((g)>>14) & 0x3e0) | (((b)>>9) & 0x7c00))
#define do_hline_inc_x \
    do {          \
        p[x]=c;  \
        x++;      \
    } while (x<x_new)
#define do_hline_dec_x \
    if (x==x_new) {    \
        p[x]=c;          \
    } else do {         \
        x--;              \
        p[x]=c;          \
    } while (x>x_new)

void gri_flat8_wire_poly_uline(int32_t c, uintptr_t parm, grs_vertex *v0, grs_vertex *v1) {
    int32_t y,y_max,x,x_new;
    fix x0,y0;
    fix x1,y1;
    fix x_fix,dx;
    uint8_t *p;

    if (gr_get_fill_type()==FILL_SOLID) c=(uint8_t )parm;
    else if (gr_get_fill_type()==FILL_CLUT) c=((uint8_t *)parm)[c];
    if (v1->y>v0->y) {
        y=fix_cint(v0->y);
        y_max=fix_cint(v1->y);
    } else {
        y=fix_cint(v1->y);
        y_max=fix_cint(v0->y);
    }
    p=grd_bm.bits+y*grd_bm.row;

    /* horizontal? */
    if (y_max-y<=1) {
        if (v1->x>v0->x) {
            x_new=fix_cint(v1->x),x=fix_cint(v0->x);
        }
        else {
            x_new=fix_cint(v0->x),x=fix_cint(v1->x);
        }
        do_hline_inc_x;
        return;
    }

    /* not horizontal */
    if (v1->y>v0->y) {
        y0=v0->y,x0=v0->x;
        y1=v1->y,x1=v1->x;
    } else {
        y1=v0->y,x1=v0->x;
        y0=v1->y,x0=v1->x;
    }
    dx=fix_div(x1-x0,y1-y0);
    x=fix_cint(x0);
    x_fix=x0+fix_mul(fix_ceil(y0)-y0,dx);
    x_new=fix_cint(x_fix);

    /* draw line */
    if (dx>=0) {
        do_hline_inc_x;
        do {
            x=x_new;
            x_new=fix_cint(x_fix+=dx);
            do_hline_inc_x;
            p+=grd_bm.row;
        } while ((++y)<y_max-1);
        x=x_new;
        x_new=fix_cint(x1);
        do_hline_inc_x;
    } else {
        do {
            do_hline_dec_x;
            x_new=fix_cint(x_fix+=dx);
            p+=grd_bm.row;
        } while ((++y)<y_max-1);
        x_new=fix_cint(x1);
        do_hline_dec_x;
    }
}
