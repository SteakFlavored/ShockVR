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
 * $Source: r:/prj/lib/src/2d/RCS/pertyp.h $
 * $Revision: 1.3 $
 * $Author: kevin $
 * $Date: 1994/07/20 23:07:19 $
 *
 * Perspective mapper public data structures.
 *
 * This file is part of the 2d library.
 *
 * $Log: pertyp.h $
 * Revision 1.3  1994/07/20  23:07:19  kevin
 * Added fill_parm as synonym for clut in grs_per_info and grs_per_setup structures.
 *
 * Revision 1.2  1994/07/04  01:37:24  kevin
 * added new structure.
 *
 * Revision 1.1  1993/12/14  22:35:12  kevin
 * Initial revision
 *
*/

/* Perspective mapper context structure. */

#ifndef __PERTYP_H
#define __PERTYP_H

#include "grs.h"

typedef struct {
    grs_point3d normal;
    grs_point3d u_grad;
    grs_point3d v_grad;
} grs_per_context;

/*********************************************************/
/* these are variables such that for screen coords x,y:  */
/* u=u0+(alpha_u*x+beta_u*y+gamma_u)/(a*x+b*y+c)            */
/* v=v0+(alpha_v*x+beta_v*y+gamma_v)/(a*x+b*y+c)            */
/* and lines of constant z are given by a*x+b*y=k          */
/* so scan_slope= -a/b for hscan, -b/a for vscan,          */
/* and its magnitude is always <= 1.                            */
/*********************************************************/
typedef struct {
    void (*scanline_func)();    /* function to do scanline.    */
    void (*shell_func)();        /* perspective mapping shell. */
    union {uint8_t *clut; uintptr_t fill_parm;};
    fix scan_slope;
    int32_t dp;
    fix alpha_u;
    fix beta_u;
    fix gamma_u;
    fix alpha_v;
    fix beta_v;
    fix gamma_v;
    fix a;
    fix b;
    fix c;
} grs_per_setup;

typedef struct {
    uint8_t *p_dst_final;
    int32_t p_dst_off;
    union {fix y_fix,x_fix;};
    fix u,du,v,dv,i,di;
    fix u0,v0;
    union {uint8_t *clut; uintptr_t fill_parm;};
    fix unum,vnum,dunum,dvnum,denom;
    fix dxl,dyl,dtl,dxr,dyr,dtr;
    fix cl,cr;
    fix scan_slope;
    int32_t dp;
    union {int32_t yp,xp;};
    union {int32_t x,y;};
    union {int32_t xl,yl;};
    union {int32_t xr,yr;};
    union {int32_t xr0,yr0;};
    int32_t u_mask,v_mask,v_shift;
    int32_t scale;
} grs_per_info;

#endif /* !__PERTYP_H */


