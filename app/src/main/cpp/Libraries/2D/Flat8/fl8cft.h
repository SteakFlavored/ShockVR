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
 * $Source: r:/prj/lib/src/2d/RCS/fl8cft.h $
 * $Revision: 1.4 $
 * $Author: kevin $
 * $Date: 1994/10/25 15:11:25 $
 *
 * Clut fill mode function table.
 * flat8 canvas.
 *
 * This file is part of the 2d library.
 */


/* FILL_NORM */
/* BMT_DEVICE */
    (ptr_type) flat8_clut_set_upixel,     /* pixel primitve */
    gr_null,     /* reserved wire poly line */
    gr_null,     /* solid hline */

    gr_null,     /* bitmap blitter */
    gr_null,     /* stencil clipped bitmap blitter */
    gr_null,     /* clut bitmap blitter */

    gr_null,     /* hflipped bitmap blitter */
    gr_null,     /* clut hflipped bitmap blitter */

    gr_null,     /* masked bitmap blitter */

    gr_null,     /* horizontal bitmap doubler */
    gr_null,     /* vertical bitmap doubler */
    gr_null,     /* horizontal and vertical bitmap doubler */

    gr_null,     /* blended horizontal bitmap doubler */
    gr_null,     /* blended vertical bitmap doubler */
    gr_null,     /* blended horizontal and vertical bitmap doubler */

    gr_null,     /* Scalers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    (ptr_type) gri_clut_poly_init,    /* solid polygon primitives */
    gr_null,

    gr_null,    /* Linear mappers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Bilinear mappers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Floor */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall2d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall1d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -hscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -vscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

/* BMT_MONO */
    gr_null,     /* solid line */
    gr_null,     /* solid wire poly line */
    gr_null,     /* gouraud hline */

    gr_null,     /* bitmap blitter */
    gr_null,     /* stencil clipped bitmap blitter */
    gr_null,     /* clut bitmap blitter */

    gr_null,     /* hflipped bitmap blitter */
    gr_null,     /* clut hflipped bitmap blitter */

    gr_null,     /* masked bitmap blitter */

    gr_null,     /* horizontal bitmap doubler */
    gr_null,     /* vertical bitmap doubler */
    gr_null,     /* horizontal and vertical bitmap doubler */

    gr_null,     /* blended horizontal bitmap doubler */
    gr_null,     /* blended vertical bitmap doubler */
    gr_null,     /* blended horizontal and vertical bitmap doubler */

    gr_null,    /* Scalers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    (ptr_type) gri_clut_spoly_init,    /* gouraud polygon primitives */
    gr_null,

    gr_null,    /* Linear mappers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Linear */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Floor */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall2d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall1d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -hscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -vscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

/* BMT_FLAT8 */
    gr_null,     /* gouraud line */
    gr_null,     /* gouraud wire poly line */
    gr_null,     /* rgb hline */

    (ptr_type) gri_flat8_fill_clut_ubitmap,      /* bitmap blitter */
    (ptr_type) gri_flat8_mask_fill_clut_bitmap,        /* stencil clipped bitmap blitter */
    (ptr_type) gri_flat8_fill_clut_ubitmap,      /* clut bitmap blitter */

    gr_null,     /* hflipped bitmap blitter */
    gr_null,     /* clut hflipped bitmap blitter */

    gr_null,     /* masked bitmap blitter */

    gr_null,     /* horizontal bitmap doubler */
    gr_null,     /* vertical bitmap doubler */
    gr_null,     /* horizontal and vertical bitmap doubler */

    gr_null,     /* blended horizontal bitmap doubler */
    gr_null,     /* blended vertical bitmap doubler */
    gr_null,     /* blended horizontal and vertical bitmap doubler */

    (ptr_type) gri_opaque_clut_scale_umap_init,    /* Scalers */
    (ptr_type) gri_trans_clut_scale_umap_init,
    (ptr_type) gri_opaque_clut_scale_umap_init,
    (ptr_type) gri_trans_clut_scale_umap_init,
    (ptr_type) gri_opaque_clut_scale_umap_init,
    (ptr_type) gri_trans_clut_scale_umap_init,

    (ptr_type) gri_clut_cpoly_init,    /* RGB polygon primitives */
    gr_null,

 (ptr_type) gr_not_imp, //### MLA- not supposed to be used    gri_opaque_clut_true_lin_umap_init,    /* Linear mappers */
    gr_null,
    gr_null,
    gr_null,
 (ptr_type) gr_not_imp, //### MLA- not supposed to be used    gri_opaque_clut_true_lin_umap_init,
    gr_null,

    (ptr_type) gri_opaque_clut_lin_umap_init,    /* Bilinear */
    (ptr_type) gri_trans_clut_lin_umap_init,
    (ptr_type) gri_opaque_clut_lin_umap_init,
    (ptr_type) gri_trans_clut_lin_umap_init,
    (ptr_type) gri_opaque_clut_lin_umap_init,
    (ptr_type) gri_trans_clut_lin_umap_init,

    (ptr_type) gri_opaque_clut_floor_umap_init,    /* Floor */
    (ptr_type) gri_trans_clut_floor_umap_init,
    (ptr_type) gri_opaque_clut_floor_umap_init,
    (ptr_type) gri_trans_clut_floor_umap_init,
    (ptr_type) gri_opaque_clut_floor_umap_init,
    (ptr_type) gri_trans_clut_floor_umap_init,

    (ptr_type) gri_opaque_clut_wall_umap_init,    /* Wall2d */
    (ptr_type) gri_trans_clut_wall_umap_init,
    (ptr_type) gri_opaque_clut_wall_umap_init,
    (ptr_type) gri_trans_clut_wall_umap_init,
    (ptr_type) gri_opaque_clut_wall_umap_init,
    (ptr_type) gri_trans_clut_wall_umap_init,

 (ptr_type) gri_opaque_clut_wall1d_umap_init,     /* Wall1d */
    (ptr_type) gri_trans_clut_wall_umap_init,
 (ptr_type) gri_opaque_clut_wall1d_umap_init,
    (ptr_type) gri_trans_clut_wall_umap_init,
 (ptr_type) gri_opaque_clut_wall1d_umap_init,
    (ptr_type) gri_trans_clut_wall_umap_init,

 (ptr_type) gri_opaque_clut_per_umap_hscan_init,    /* Perspective -hscan */
 (ptr_type) gri_trans_clut_per_umap_hscan_init,
 (ptr_type) gri_opaque_clut_per_umap_hscan_init,
 (ptr_type) gri_trans_clut_per_umap_hscan_init,
 (ptr_type) gri_opaque_clut_per_umap_hscan_init,
 (ptr_type) gri_trans_clut_per_umap_hscan_init,

 (ptr_type) gri_opaque_clut_per_umap_vscan_init,    /* Perspective -vscan */
 (ptr_type) gri_trans_clut_per_umap_vscan_init,
 (ptr_type) gri_opaque_clut_per_umap_vscan_init,
 (ptr_type) gri_trans_clut_per_umap_vscan_init,
 (ptr_type) gri_opaque_clut_per_umap_vscan_init,
 (ptr_type) gri_trans_clut_per_umap_vscan_init,

/* BMT_FLAT24 */
    gr_null,     /* rgb line */
    gr_null,     /* rgb wire poly line */
    gr_null,     /* solid vline */

    gr_null,     /* bitmap bclutter */
    gr_null,     /* stencil clipped bitmap bclutter */
    gr_null,     /* clut bitmap bclutter */

    gr_null,     /* hflipped bitmap bclutter */
    gr_null,     /* clut hflipped bitmap bclutter */

    gr_null,     /* masked bitmap bclutter */

    gr_null,     /* horizontal bitmap doubler */
    gr_null,     /* vertical bitmap doubler */
    gr_null,     /* horizontal and vertical bitmap doubler */

    gr_null,     /* blended horizontal bitmap doubler */
    gr_null,     /* blended vertical bitmap doubler */
    gr_null,     /* blended horizontal and vertical bitmap doubler */

    gr_null,    /* Scalers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    (ptr_type) gri_clut_tpoly_init,    /* translucent polygon primitives */
    gr_null,

    gr_null,    /* Linear mappers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Linear */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Floor */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall2d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall1d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -hscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -vscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

/* BMT_RSD8 */
    gr_null,     /* translucent line */
    gr_null,     /* translucent wire poly line */
    gr_null,     /* gouraud vline */

    (ptr_type) gri_gen_rsd8_ubitmap,     /* bitmap blitter */
    (ptr_type) gri_gen_rsd8_bitmap,     /* stencil clipped bitmap blitter */
    gr_null,     /* clut bitmap bclutter */

    gr_null,     /* hflipped bitmap bclutter */
    gr_null,     /* clut hflipped bitmap bclutter */

    gr_null,     /* masked bitmap bclutter */

    gr_null,     /* horizontal bitmap doubler */
    gr_null,     /* vertical bitmap doubler */
    gr_null,     /* horizontal and vertical bitmap doubler */

    gr_null,     /* blended horizontal bitmap doubler */
    gr_null,     /* blended vertical bitmap doubler */
    gr_null,     /* blended horizontal and vertical bitmap doubler */

    (ptr_type) rsd8_tm_init,    /* Scalers */
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,

    (ptr_type) gri_clut_stpoly_init,    /* shaded translucent polygon primitives */
    gr_null,

    (ptr_type) rsd8_tm_init, /* Linear mappers */
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,

    (ptr_type) rsd8_tm_init,    /* Linear */
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,

    (ptr_type) rsd8_tm_init,    /* Floor */
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,

    (ptr_type) rsd8_tm_init,    /* Wall2d */
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,

    (ptr_type) rsd8_tm_init,    /* Wall1d */
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,
    (ptr_type) rsd8_tm_init,

    (ptr_type) rsd8_pm_init,    /* Perspective -hscan */
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,

    (ptr_type) rsd8_pm_init,    /* Perspective -vscan */
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,
    (ptr_type) rsd8_pm_init,

/* BMT_TLUC8 */
    gr_null,     /* shaded translucent line */
    gr_null,     /* shaded translucent wire poly line */
    gr_null,     /* rgb vline */

    gr_null,     /* bitmap bclutter */
    gr_null,     /* stencil clipped bitmap bclutter */
    gr_null,     /* clut bitmap bclutter */

    gr_null,     /* hflipped bitmap bclutter */
    gr_null,     /* clut hflipped bitmap bclutter */

    gr_null,     /* masked bitmap bclutter */

    gr_null,     /* horizontal bitmap doubler */
    gr_null,     /* vertical bitmap doubler */
    gr_null,     /* horizontal and vertical bitmap doubler */

    gr_null,     /* blended horizontal bitmap doubler */
    gr_null,     /* blended vertical bitmap doubler */
    gr_null,     /* blended horizontal and vertical bitmap doubler */

    (ptr_type) gri_tluc8_opaque_clut_scale_umap_init,    /* Scalers */
    (ptr_type) gri_tluc8_trans_clut_scale_umap_init,
    (ptr_type) gri_tluc8_opaque_clut_scale_umap_init,
    (ptr_type) gri_tluc8_trans_clut_scale_umap_init,
    (ptr_type) gri_tluc8_opaque_clut_scale_umap_init,
    (ptr_type) gri_tluc8_trans_clut_scale_umap_init,

    gr_null,
    gr_null,

    gr_null,    /* Linear mappers */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    (ptr_type) gri_tluc8_opaque_clut_lin_umap_init,    /* Linear */
    (ptr_type) gri_tluc8_trans_clut_lin_umap_init,
    (ptr_type) gri_tluc8_opaque_clut_lin_umap_init,
    (ptr_type) gri_tluc8_trans_clut_lin_umap_init,
    (ptr_type) gri_tluc8_opaque_clut_lin_umap_init,
    (ptr_type) gri_tluc8_trans_clut_lin_umap_init,

    gr_null,        /* Floor */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall2d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Wall1d */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -hscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null,

    gr_null,    /* Perspective -vscan */
    gr_null,
    gr_null,
    gr_null,
    gr_null,
    gr_null

