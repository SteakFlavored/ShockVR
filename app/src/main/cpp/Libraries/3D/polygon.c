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
//
// $Source: r:/prj/lib/src/3d/RCS/polygon.asm $
// $Revision: 1.31 $
// $Author: jaemz $
// $Date: 1994/11/06 13:59:23 $
//
// Polygon drawers
//

#include "lg.h"
#include "3d.h"
#include "globalv.h"

// prototypes
int32_t check_and_draw_common(int32_t c,int32_t n_verts,g3s_phandle *p);
int32_t draw_poly_common(int32_t c,int32_t n_verts,g3s_phandle *p);
int32_t draw_line_common(g3s_phandle p0,g3s_phandle p1);

#define GR_WIRE_POLY_LINE     6
#define GR_WIRE_POLY_SLINE  7
#define GR_WIRE_POLY_CLINE  8

#define MAX_VERTS 100 // max for one poly

// array of 2d points
grs_vertex    p_vlist[MAX_VERTS];
grs_vertex    *p_vpl[MAX_VERTS];
int32_t                _n_verts;
int32_t                poly_color;

// arrays of point handles, used in clipping
g3s_phandle        vbuf[MAX_VERTS];
g3s_phandle        _vbuf2[MAX_VERTS];

// used by line clipper
/*int32_t        temp_points[4];
int32_t        n_temp_used;*/

int32_t        draw_color;
int32_t        poly_index[] = {FIX_UPOLY,FIX_TLUC8_UPOLY,FIX_USPOLY,FIX_TLUC8_SPOLY,FIX_UCPOLY};

int8_t        gour_flag;         // 0=normal,1=tluc_poly,2=spoly,3=tluc_spoly,4=cpoly

// check if a list of point (as in a polygon) are on screen. returns codes
// takes esi=list of points, ecx=codes, returns bx=codes.
// trashes ebx,ecx,edx,esi
g3s_codes g3_check_codes(int32_t n_verts, g3s_phandle *p)
 {
     int32_t                        i;
     g3s_codes            retcode;
     int8_t                    andcode,orcode;

     andcode = 0xff;
     orcode = 0;

     for (i=n_verts; i>0; i--)
      {
          andcode &= (*p)->codes;
          orcode |= (*p)->codes;
          p++;
      }

     retcode.or = orcode;
     retcode.and = andcode;
     return(retcode);
 }

extern void g3_compute_normal_quick(g3s_vector *v, g3s_vector *v0,g3s_vector *v1,g3s_vector *v2);

// takes 3 rotated points: eax,edx,ebx.
// returns al=true (& s flag set) if facing. trashes all but ebp
bool g3_check_poly_facing(g3s_phandle p0,g3s_phandle p1,g3s_phandle p2)
{
    g3s_vector temp_vector;
    g3_compute_normal_quick(&temp_vector, (g3s_vector *) p0, (g3s_vector *) p1, (g3s_vector *) p2);

    int64_t result = (int64_t)p0->gX * (int64_t)temp_vector.gX +
            (int64_t)p0->gY * (int64_t)temp_vector.gY +
            (int64_t)p0->gZ * (int64_t)temp_vector.gZ;
    return result < 0;
}

// takes same input as draw_poly, but first checks if facing
int32_t g3_check_and_draw_cpoly(int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 4;
     return(check_and_draw_common(0,n_verts,p));
 }

int32_t g3_check_and_draw_tluc_spoly(int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 3;
     return(check_and_draw_common(0,n_verts,p));
 }

int32_t g3_check_and_draw_spoly(int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 2;
     return(check_and_draw_common(0,n_verts,p));
 }

int32_t g3_check_and_draw_tluc_poly(int32_t c,int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 1;
     return(check_and_draw_common(c,n_verts,p));
 }

int32_t g3_check_and_draw_poly(int32_t c,int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 0;
     return(check_and_draw_common(c,n_verts,p));
 }

int32_t check_and_draw_common(int32_t c,int32_t n_verts,g3s_phandle *p)
 {
#ifdef stereo_on
          test     _g3d_stereo,1
          jz        check_and_draw_common_raw
          pushm eax,ecx,esi

          call check_and_draw_common_raw
          set_rt_canv

          popm eax,ecx,esi
          pushm eax,ecx

          // moves list at esi to temp and repoints esi
          test gour_flag,6
          jnz  do_uvi_copy1
          move_to_stereo
          jmp      raw_poly_continue1
do_uvi_copy1:
          mov      edx,esi
          mov      eax,ecx
          move_to_stereo_and_uvi
          mov      esi,edx
raw_poly_continue1:

          popm eax,ecx
          call check_and_draw_common_raw

          set_lt_canv
          ret
check_and_draw_common_raw:
#endif

    if (g3_check_poly_facing(p[0],p[1],p[2]))
     {
#ifdef stereo_on
     js        draw_poly_common_raw
#else
        return draw_poly_common(c,n_verts,p);
#endif
     }
    else
        return 0;    // no draw
 }

// takes ecx=# verts, esi=ptr to list of point handles
// modify all but ebp

int32_t g3_draw_cpoly(int32_t n_verts,g3s_phandle *p)          //RBG-space smooth poly
 {
     gour_flag = 4;
     return draw_poly_common(0,n_verts,p);
 }

int32_t g3_draw_tluc_spoly(int32_t n_verts,g3s_phandle *p)    //smooth poly
 {
     gour_flag = 3;
     return draw_poly_common(0,n_verts,p);
 }

int32_t g3_draw_spoly(int32_t n_verts,g3s_phandle *p)                 //smooth poly
 {
     gour_flag = 2;
     return draw_poly_common(0,n_verts,p);
 }

int32_t g3_draw_tluc_poly(int32_t c,int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 1;
     return draw_poly_common(c,n_verts,p);
 }

int32_t g3_draw_poly(int32_t c,int32_t n_verts,g3s_phandle *p)
 {
     gour_flag = 0;
     return draw_poly_common(c,n_verts,p);
 }


int32_t draw_poly_common(int32_t c,int32_t n_verts,g3s_phandle *p)
 {
     int8_t                    andcode,orcode;
    g3s_phandle        *old_p;
    int32_t                        i;
    g3s_phandle        *src;
    g3s_phandle        src_pt;
    grs_vertex        *dest;
    int32_t                     rgb;

    poly_color = c;

// first, go through points and get codes
     andcode = 0xff;
     orcode = 0;
     old_p = p;

     for (i=n_verts; i>0; i--)
      {
          andcode &= (*p)->codes;
          orcode |= (*p)->codes;
          p++;
      }

    if (andcode) return CLIP_ALL;    // punt!

    p = old_p;

// copy to temp buffer for clipping
    memmove(vbuf,p,n_verts<<2);

    n_verts = g3_clip_polygon(n_verts,vbuf,_vbuf2);
    if (!n_verts)
         return CLIP_ALL;

// now, copy 2d points to buffer for polygon draw, projecting if neccesary
    src = _vbuf2;
    dest = p_vlist;

    for (i=0; i<n_verts; i++)
     {
         src_pt = *(src++);

         // check if this point has been projected
        if ((src_pt->p3_flags & PF_PROJECTED) == 0)    // projected yet?
            g3_project_point(src_pt);

        dest->x = src_pt->sx;    // store 2D X & Y
        dest->y = src_pt->sy;
         p_vpl[i] = dest;            // store ptr
         dest++;
     }

    if (gour_flag>=2)    // some kind of shading
     {
         if (gour_flag>=4)    // cpoly
          {
            src = _vbuf2;
            dest = p_vlist;
            for (i=0; i<n_verts; i++)
             {
                 src_pt = *(src++);
                 rgb = src_pt->rgb;
                 dest->u = (rgb & 0x000003ff) << 14;    // r
                 dest->v = (rgb & 0x001ffc00) << 3;    // g
                 dest->w = (rgb & 0xffe00000) >> 8;    // b

                 dest++;
             }
          }
         else    // spoly
          {
            src = _vbuf2;
            dest = p_vlist;
            for (i=0; i<n_verts; i++)
             {
                 src_pt = *(src++);
                 dest->i = (((uint32_t) src_pt->i) + gouraud_base) << 8;

                 dest++;
             }
          }
     }

// draw it
    ((void (*)(int32_t c,int32_t n,grs_vertex **vpl)) grd_canvas_table[poly_index[gour_flag]])
                         (poly_color,n_verts, p_vpl);

     return CLIP_NONE;
 }


// draw a point in 3-space. takes esi=point. returns al=drew.
// trashes eax,edx,esi and if must project, ecx
int32_t g3_draw_point(g3s_phandle p)
 {
     int32_t     sx,sy;

     if (p->codes) return CLIP_ALL;

     if ((p->p3_flags & PF_PROJECTED) == 0)     // check if projected
        g3_project_point(p);

     sx = (p->sx + 0x08000) >> 16;    // round & get int32_t part
     sy = (p->sy + 0x08000) >> 16;    // round & get int32_t part
     return (((int32_t (*)(int16_t x,int16_t y))grd_canvas_table[DRAW_POINT])(sx,sy));
 }

// draws a line in 3-space. takes esi,edi=points

// fixed 7/24 dc to have a common and have draw_line set gour_flag, not ignore it
int32_t g3_draw_cline(g3s_phandle p0,g3s_phandle p1) // rgb-space gouraud line
 {
     if (p0->rgb!=p1->rgb)
      {
          gour_flag = 1;
          return(draw_line_common(p0,p1));
      }
     else
      {
          gour_flag = 0;
          draw_color = grd_ipal[gr_index_brgb(p0->rgb)];
         return(draw_line_common(p0,p1));
      }
 }


int32_t g3_draw_sline(g3s_phandle p0,g3s_phandle p1) // 2d-intensity gouraud line
 {
     gour_flag = -1;
     return(draw_line_common(p0,p1));
 }

int32_t g3_draw_line(g3s_phandle p0,g3s_phandle p1)
 {
     draw_color = gr_get_fcolor();
     gour_flag = 0;
     return(draw_line_common(p0,p1));
 }

int32_t draw_line_common(g3s_phandle p0,g3s_phandle p1)
 {
     int8_t                code0,code1;
    int32_t result = CLIP_NONE;
    grs_vertex    v0,v1;

    vbuf[0] = p0;
    vbuf[1] = p1;
    if (g3_clip_line(vbuf,_vbuf2)==16) return CLIP_ALL;

    p0 = _vbuf2[0];
    p1 = _vbuf2[1];
    code0 = p0->codes;
    code1 = p1->codes;

// ok, draw now with points = esi,edi. bl=codes_or
// note that in stereo mode, you're doing this twice.  We should
// just always project all points, or have the code clipper update stuff

    if ((p0->p3_flags & PF_PROJECTED) == 0)
         g3_project_point(p0);
    if ((p1->p3_flags & PF_PROJECTED) == 0)
         g3_project_point(p1);

    if (draw_color==255) draw_color=0;

     if (gour_flag==0)    // normal line
      {
// use wire poly lines.  Always clip.
// set up args -- vertex contents on stack, pass sp
// for line only need 1st 2 elements of grs_vertex, only push them
            v0.x = p0->sx;
            v0.y = p0->sy;
            v1.x = p1->sx;
            v1.y = p1->sy;
            ((int32_t (*)(int32_t c, int32_t parm, grs_vertex *v0, grs_vertex *v1))grd_line_clip_fill_vector[GR_WIRE_POLY_LINE])
                                (draw_color,gr_get_fill_parm(),&v0,&v1);

            result = CLIP_NONE;
      }
     else if (gour_flag>0)    // cline
      {
          uint8_t     a,b,c;

            v0.x = p0->sx;
            v0.y = p0->sy;
            gr_split_rgb (p0->rgb, &a, &b, &c);
            v0.u = a; v0.v = b; v0.w = c;

            v1.x = p1->sx;
            v1.y = p1->sy;
            gr_split_rgb (p1->rgb, &a, &b, &c);
            v1.u = a; v1.v = b; v1.w = c;
            ((int32_t (*)(int32_t c, int32_t parm, grs_vertex *v0, grs_vertex *v1))grd_line_clip_fill_vector[GR_WIRE_POLY_CLINE])
                                (gr_get_fcolor(),gr_get_fill_parm(),&v0,&v1);

            result = CLIP_NONE;
//          Warning("implement me?");
/*
//          mov      edx,ebx                      // dl=clip codes

// set up args -- vertex contents on stack, pass sp
// for cline only need 1st 5 elements of grs_vertex, only push them
    gr_splitrgb [esi].rgb,eax    // eax scratch
    pushm    [esi].sy,[esi].sx
    mov     ebx,esp            // v0 on stack, addr is arg
    gr_splitrgb [edi].rgb,eax    // eax scratch
    pushm    [edi].sy,[edi].sx
    mov     ecx,esp            // v1 on stack, addr is arg

    gr_getcol    eax
    gr_getfp    edx
          mov      edi,grd_line_clip_fill_vector
    call     d [edi + 4*GR_WIRE_POLY_CLINE]
    add esp,40            // 2 vertex's each 5 fix's

          mov      eax,CLIP_NONE
          jmp      leave_draw_line*/
      }
     else    // sline
      {
          Warning("implement me?");
// we have to do this annoyingly because i is an sfix,
// and 2d takes a fix, so we dump things in eax and munge
/*
// new smaller converter
          xor      eax,eax
          mov      ax,[edi].i
          shl      eax,8
          push     eax
          push     [edi].sy
          xor      eax,eax
          mov      ax,[esi].i
          shl      eax,8

          or        bl,bl    // check triv acc

          mov      ebx,eax            // we had to do all the ugliness in eax b/c needed
                                          // the codes in bl to check for triv acc, and we
                                          // couldnt do the check earlier because all our
                                          // and's reset the zero flag needed for the jz below
          mov      eax,[esi].sx
          mov      edx,[esi].sy
          mov      ecx,[edi].sx

          jz        unclipped_sline
                call gen_fix_sline_
          jmp      leave_draw_line

          ret

unclipped_sline:
//          gr_call FIX_USLINE
// set up args -- vertex contents on stack, pass sp
// pushd contents of vertex -- don't care about u,v,w
// i needs to have sfix to fix
          xor      eax,eax            // eax scratch
          mov      ax,[esi].i
          shl      eax,8
          push     eax
    pushm    0,0,0            // uvw are don't cares
    pushm    [esi].sy,[esi].sx
    mov     ebx,esp            // v0 on stack, addr is arg
          xor      eax,eax            // eax scratch
          mov      ax,[edi].i
          shl      eax,8
          push     eax
    pushm    0,0,0            // uvw are don't cares
    pushm    [edi].sy,[edi].sx
    mov     ecx,esp            // v1 on stack, addr is arg
//     now its OK to trash esi
    gr_getcol    eax
    gr_getfp    edx
    mov    esi,grd_uline_fill_vector
    call     d [esi + 4*SLINE]
    add esp,48            // 2 vertex's each 6 fix's

//    junk from old stuff -- kill these
    pop esi
    pop esi

          mov      eax,CLIP_NONE
          jmp      leave_draw_line
*/
      }

     return result;
 }

// check if a surface is facing the viewer
// takes esi=point on surface, edi=surface normal (can be unnormalized)
// trashes eax,ebx,ecx,edx. returns al=true & sign set, if facing
bool g3_check_normal_facing(g3s_vector *v,g3s_vector *normal)
{
    int64_t result = (int64_t)(v->gX - _view_position.gX) * (int64_t)normal->gX +
            (int64_t)(v->gY - _view_position.gY) * (int64_t)normal->gY +
            (int64_t)(v->gZ - _view_position.gZ) * (int64_t)normal->gZ;
    return result < 0;
}
