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
 * $Source: r:/prj/cit/src/RCS/cardmfd.c $
 * $Revision: 1.9 $
 * $Author: xemu $
 * $Date: 1994/10/16 15:51:58 $
 *
 */

#include <string.h>

#include "mfdint.h"
#include "mfdext.h"
#include "mfddims.h"
#include "objsim.h"
#include "gamestrn.h"
#include "tools.h"
#include "objprop.h"
#include "colors.h"
#include "fullscrn.h"
#include "cit2d.h"

#include "mfdart.h"
#include "gamescr.h"
#include "otrip.h"
#include "cybstrng.h"
#include "gr2ss.h"

void mfd_accesscard_expose(MFD* mfd, uint8_t control);

// ============================================================
//                         ACCESS CARD MFD
// ============================================================

// ---------------
// EXPOSE FUNCTION
// ---------------


#define MFD_CARD_FUNC 24

#define Y_STEP 5
#define DISPLAY_TOP_MARGIN 15
#define LEFT_X 2
#define RIGHT_X (MFD_VIEW_WID - 2)
#define CODES_WID (RIGHT_X - LEFT_X + 1)
#define LAST_BITS(mfd) (*(uint32_t*)&(player_struct.mfd_func_data[MFD_CARD_FUNC][mfd*sizeof(int32_t)]))
#define ITEM_COLOR 0x5A

void mfd_accesscard_expose(MFD* mfd, uint8_t control)
{
    bool full = control & MFD_EXPOSE_FULL;
    if (control == 0)  // MFD is drawing stuff
    {
        // Do unexpose stuff here.
    }
    if (control & MFD_EXPOSE) // Time to draw stuff
    {
        extern int32_t mfd_bmap_id(int32_t);
        int32_t i;
        uint32_t lastbits = LAST_BITS(mfd->id);
        uint32_t bits = 0;
        // clear update rects
        mfd_clear_rects();
        // set up canvas
        gr_push_canvas(pmfd_canvas);
        ss_safe_set_cliprect(0,0,MFD_VIEW_WID,MFD_VIEW_HGT);

        if (!full_game_3d)
//KLC - chg for new art            ss_bitmap(&mfd_background, 0, 0);
            gr_bitmap(&mfd_background, 0, 0);
        if (full)
        {
            mfd_draw_string(get_object_long_name(GENCARDS_TRIPLE,NULL,0),1,1,GREEN_YELLOW_BASE,true);
        }
        i = mfd_bmap_id(GENCARDS_TRIPLE);
        draw_raw_resource_bm(i,(MFD_VIEW_WID-res_bm_width(i))/2,
            DISPLAY_TOP_MARGIN+(MFD_VIEW_HGT-DISPLAY_TOP_MARGIN-res_bm_height(i))/2);

        // find the "access cards" object
        for (i = 0; i < NUM_GENERAL_SLOTS; i++)
        {
            ObjID obj = player_struct.inventory[i];
            if (obj != OBJ_NULL && ID2TRIP(obj) == GENCARDS_TRIPLE)
            {
                bits = objSmallstuffs[objs[obj].specID].data1;
                break;
            }
        }

        // I wonder if the ||= operator exists.
        full = full || bits != lastbits;
        // Lets see what access codes we have.
        if (full)
        {
            int16_t x = LEFT_X;
            int16_t y = DISPLAY_TOP_MARGIN;
            int16_t w,h;
            bool old_wrap = mfd_string_wrap;
            int8_t buf[256] = "";
            int8_t *s = buf;
            for (i = 1; i <= sizeof(uint32_t)*8; i++)
            {
                if (bits & (1 << i))
                {
                    strcpy(s,get_temp_string(MKREF(RES_accessCards,i<<1)));
                    s += strlen(s);
                    *(s++) = ' ';
                }

            }
            *s = '\0';
            gr_set_font((grs_font*)ResLock(MFD_FONT));
            mfd_string_wrap = false;
            wrap_text(buf,CODES_WID);
            gr_string_size(buf,&w,&h);
            x += (CODES_WID - w)/2;
            y += (MFD_VIEW_HGT - h)/2 - Y_STEP;
            mfd_full_draw_string(buf,x,y,ITEM_COLOR,MFD_FONT,true,true);
            mfd_string_wrap = old_wrap;
            ResUnlock(MFD_FONT);
            LAST_BITS(mfd->id) = bits;
        }
        if (full)
            mfd_add_rect(0,0,MFD_VIEW_WID,MFD_VIEW_HGT);


        // Pop the canvas
        gr_pop_canvas();
        // Now that we've popped the canvas, we can send the
        // updated mfd to screen
        mfd_update_rects(mfd);

    }

}


