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
 * $Source: r:/prj/cit/src/RCS/olh.c $
 * $Revision: 1.27 $
 * $Author: dc $
 * $Date: 1994/11/21 09:02:38 $
 *
 */

#include <stdlib.h>
#include <string.h>

#include "Prefs.h"

#include "player.h"
#include "gamestrn.h"
#include "objapp.h"
#include "objects.h"
#include "objprop.h"
#include "gameobj.h"
#include "hudobj.h"
#include "hud.h"
#include "mainloop.h"
#include "gamescr.h"
#include "olhint.h"
#include "faketime.h"
#include "objbit.h"
#include "objuse.h"
#include "doorparm.h"
#include "render.h"
#include "strwrap.h"
#include "tools.h"
#include "screen.h"
#include "fullscrn.h"
#include "input.h"
#include "grenades.h"
#include "mfdext.h"
#include "olhext.h"
#include "cit2d.h"
#include "gr2ss.h"
#include "hkeyfunc.h"
#include "status.h"

#include "otrip.h"
#include "cybstrng.h"

// -------------------------------------------------
//          ON-LINE HELP FOR SYSTEM SHOCK
// -------------------------------------------------

bool olh_active = true;
bool olh_overlay_on = false;
olh_data olh_object = { OBJ_NULL, { 0, 0} };


// ---------
// INTERNALS
// ---------

int8_t* get_olh_string(ObjID obj,int8_t* buf);
LGPoint draw_olh_string(int8_t* s, int16_t xl, int16_t yl);
void olh_do_panel_ref(int16_t xl, int16_t yl);
void olh_do_callout(int16_t xl, int16_t yl);
bool is_compound_use_obj(ObjID obj);
void olh_do_cursor(int16_t xl, int16_t yl);


#define IS_SCREEN(obj) (objs[obj].obclass == CLASS_BIGSTUFF                     	\
                        && objs[obj].subclass == BIGSTUFF_SUBCLASS_ONTHEWALL   	\
                        && (objs[obj].info.type == TRIP2TY(SCREEN_TRIPLE)           	\
                            || objs[obj].info.type == TRIP2TY(SUPERSCREEN_TRIPLE) 	\
                            || objs[obj].info.type == TRIP2TY(BIGSCREEN_TRIPLE)))


bool olh_candidate(ObjID obj)
{
   bool check_dist = false;
   bool retval = false;

   if (objs[obj].info.inst_flags & OLH_INST_FLAG)
      return false;
   switch(ID2TRIP(obj))
   {
      case CAMERA_TRIPLE:
      case LARGCPU_TRIPLE:
         check_dist = false;
         retval = true;
         break;
      default:
         if(USE_MODE(obj)==NULL_USE_MODE) return false;
         break;
   }
   switch (objs[obj].obclass)
   {
   case CLASS_DOOR:
      if ((ID2TRIP(obj) == LABFORCE_TRIPLE) || (ID2TRIP(obj) == RESFORCE_TRIPLE))
         return false;
      check_dist = !door_locked(obj) && !door_moving(obj,false)
               && DOOR_REALLY_CLOSED(obj);
      break;
   case CLASS_BIGSTUFF:
      if (IS_SCREEN(obj))
      {
         extern int8_t camera_map[NUM_HACK_CAMERAS];
         extern ObjID hack_cam_objs[NUM_HACK_CAMERAS];
         ObjSpecID sid = objs[obj].specID;
         int16_t v = objBigstuffs[sid].data2 & 0x7F;
         if ((v >= FIRST_CAMERA_TMAP) && (v <= FIRST_CAMERA_TMAP + NUM_HACK_CAMERAS))
         {
            if (camera_map[NUM_HACK_CAMERAS] && hack_cam_objs[v - FIRST_CAMERA_TMAP])
               check_dist = true;
         }
         break;
      }
      if (ID2TRIP(obj) == SURG_MACH_TRIPLE)
      {
         check_dist = true;
         break;
      }
      if (((ObjProps[OPNUM(obj)].flags & CLASS_FLAGS) >> CLASS_FLAGS_SHF) == STUFF_OBJUSE_FLAG)
      {
         if (objBigstuffs[objs[obj].specID].data1 != 0)
            check_dist = true;
         break;
      }
   case CLASS_SMALLSTUFF:
      if (USE_MODE(obj) == USE_USE_MODE)
      {
         check_dist = true;
         break;
      }
      if (USE_MODE(obj) == PICKUP_USE_MODE)
      {
         if (ObjProps[OPNUM(obj)].flags & INVENTORY_GENERAL)
            check_dist = true;
         break;
      }
      // smallstuff falls through to default.
   default:
      if (USE_MODE(obj) == PICKUP_USE_MODE
         || USE_MODE(obj) == USE_USE_MODE)
         check_dist = true;
      break;

   }

   if (check_dist)
   {
      int32_t mode = USE_MODE(obj);
      fix crit = (mode == PICKUP_USE_MODE) ? MAX_PICKUP_DIST : MAX_USE_DIST;

      if (check_object_dist(obj,PLAYER_OBJ,crit))
         retval = true;
   }
   return retval;
}

int16_t use_mode_idx[] = {
                         REFINDEX(REF_STR_helpTake),
                         REFINDEX(REF_STR_helpUse),
                         -1,
                         -1,
                       };

int16_t weap_subclass_idx[] =
{
   REFINDEX(REF_STR_helpAttackGun),
   REFINDEX(REF_STR_helpAttackAuto),
   REFINDEX(REF_STR_helpAttackGun),
   REFINDEX(REF_STR_helpAttackHTH),
   REFINDEX(REF_STR_helpAttackGun),
   REFINDEX(REF_STR_helpAttackGun),
};



extern bool is_container(ObjID id, int32_t** d1, int32_t ** d2);


// basically we chose a string id for a string
// that has  %s, and lg_strintf the name into the
// the string.
int8_t* get_olh_string(ObjID obj,int8_t* buf)
{

   int32_t *d1,*d2;
   int32_t obclass = objs[obj].obclass;
   Ref r = 0;
   int16_t mode;

   switch (obclass)
   {
      case CLASS_CRITTER:
      {
         int32_t w = player_struct.actives[ACTIVE_WEAPON];
         int32_t type = player_struct.weapons[w].type;
         if (type == EMPTY_WEAPON_SLOT)
            return strcpy(buf,get_object_long_name(ID2TRIP(obj),NULL,0));
         r = MKREF(RES_olh_strings,weap_subclass_idx[type]);
         goto got_id;
      }
      case CLASS_DOOR:
         r = REF_STR_helpDoor;
         goto got_id;
      default:
         break;
   }
   switch (ID2TRIP(obj))
   {
      case CAMERA_TRIPLE:
      case LARGCPU_TRIPLE:
         r = REF_STR_helpSecurity;
         goto got_id;
   }
   mode = USE_MODE(obj);
   if (is_container(obj,&d1,&d2) && mode == USE_USE_MODE)
   {
      r = REF_STR_helpSearch;
      goto got_id;
   }
   if (use_mode_idx[mode] != -1)
   {
      r = MKREF(RES_olh_strings,use_mode_idx[mode]);
      goto got_id;
   }
   // more cases go here...

got_id:
   if (r != 0)
   {
      int8_t* s = (int8_t *)RefLock(r);
      lg_sprintf(buf, s, get_object_long_name(ID2TRIP(obj),NULL,0));
      RefUnlock(r);
   }
   return buf;

}





// ---------
// EXTERNALS
// ---------
extern bool	DoubleSize;

//-----------------------------
// olh_scan_objects()
//
// This gets called to detect objects in front of the player that have
// help strings.  It sets up for olh_do_hudobjs.

extern void olh_scan_objs(void);





#define SCAN_FREQ_SHF (APPROX_CIT_CYCLE_SHFT - 2)

void olh_scan_objects(void)
{
   static uint32_t last_scan = 0;

   if (*tmd_ticks >> SCAN_FREQ_SHF <= last_scan)
      return;
   if (input_cursor_mode == INPUT_OBJECT_CURSOR)
      return;
   if (player_struct.panel_ref != OBJ_NULL)
      return;
   olh_scan_objs();
   if (olh_object.obj == OBJ_NULL)
      return;
#ifdef SET_HUDOBJ
   if (hudobj_rect_capable(olh_object.obj))
   {
      hudobj_set_id(olh_object.obj,true);
   }
#endif // SET_HUDOBJ
}


LGPoint draw_olh_string(int8_t* s, int16_t xl, int16_t yl)
{
   int16_t w,h;
   int16_t x,y;

   string_replace_char(s,'\n',CHAR_SOFTSP);
   gr_set_font((grs_font*)ResGet(RES_tinyTechFont));
   gr_set_fcolor(hud_colors[hud_color_bank][2]);
   wrap_text(s,OLH_WRAP_WID);
   gr_string_size(s,&w,&h);
   ss_point_convert(&xl,&yl,true);
   if (DoubleSize)
   {
      xl *= 2;
      yl = yl*2 + 1;
   }
   x = SCREEN_VIEW_X - xl + SCREEN_VIEW_WIDTH - w - 1;
   y = SCREEN_VIEW_Y - yl + SCREEN_VIEW_HEIGHT - h - 1;
   draw_shadowed_string(s,x,y,true);
   return MakePoint(x-1, y+(h/2));
}

uint16_t fixture_panel_stringrefs [] =
{
   REFINDEX(REF_STR_helpPanel),
   REFINDEX(REF_STR_helpPanel),
   REFINDEX(REF_STR_helpPanel),
   REFINDEX(REF_STR_helpPanel),
   REFINDEX(REF_STR_helpElevator),
   REFINDEX(REF_STR_helpElevator),
   REFINDEX(REF_STR_helpElevator),
   REFINDEX(REF_STR_helpKeypad),
   REFINDEX(REF_STR_helpKeypad),
   REFINDEX(REF_STR_helpPanel),
   REFINDEX(REF_STR_helpPanel),
};

extern int8_t *get_object_lookname(ObjID, int8_t*, int);

void olh_do_panel_ref(int16_t xl, int16_t yl)
{
   int32_t *d1, *d2;
   ObjID obj = player_struct.panel_ref;
   int8_t buf[80];

   buf[0] = '\0';
   if (is_container(obj,&d1,&d2) && (*d1 != 0 || *d2 != 0))
   {
      int8_t namebuf[80];

      get_object_lookname(obj,namebuf,sizeof(namebuf));
      lg_sprintf(buf,get_temp_string(REF_STR_helpGump),namebuf);
   }
   else if (objs[obj].obclass == CLASS_FIXTURE)
   {
      Ref ref = 0;
      if (objs[obj].subclass == FIXTURE_SUBCLASS_CONTROL)
         ref = REF_STR_helpSwitch;
      else if (objs[obj].subclass == FIXTURE_SUBCLASS_PANEL)
      {
         extern bool comparator_check(int32_t comparator, ObjID obj, uint8_t *special_code);
         uint8_t special;
         ObjFixture* pfixt = &objFixtures[objs[obj].specID];
         bool rv = comparator_check(pfixt->comparator, obj, &special);
         if (special == 0)
            ref = MKREF(RES_olh_strings,fixture_panel_stringrefs[objs[obj].info.type]);
      }
      if (ref != 0)
         get_string(ref,buf,sizeof(buf));
   }
   if (buf[0] != '\0')
      draw_olh_string(buf,xl,yl);
}

void olh_do_callout(int16_t xl, int16_t yl)
{
   int32_t best_rect = -1;
   ObjID obj = olh_object.obj;

   if (obj == OBJ_NULL)
      return;
#ifdef SET_HUDOBJ
   if (hudobj_rect_capable(ID2TRIP(obj)))
   {
      int32_t j;
      for (j = 0; j < current_num_hudobjs; j++)
      {
         struct _hudobj_data *dat = &hudobj_vec[j];
         if (dat->id == obj)
         {
            best_rect = j;
            break;
         }
      }
      /* perhaps in studlier versions we'll do computations
         to decide whether we're no longer a candidate,
         rather than just blow away its candidacy */
      hudobj_set_id(obj,false);
      if (best_rect == -1)
         olh_object.obj = OBJ_NULL;
   }
#endif // SET_HUDOBJ
//   if (obj != OBJ_NULL)
   {
      int8_t buf[80];
      int8_t* s = get_olh_string(obj,buf);
      LGPoint spos = draw_olh_string(s,xl,yl);
      LGPoint pos = olh_object.loc;
      pos.x = (int32_t)(pos.x+1)*SCAN_RATIO;
      pos.y = (int32_t)(pos.y+1)*SCAN_RATIO;
      if (DoubleSize)
      {
      	pos.x *= 2;
      	pos.y *= 2;
      }
      ss_int_line(spos.x - 1,spos.y - 1,pos.x,pos.y);
   }
   if (best_rect != -1)
   {
      struct _hudobj_data *dat = &hudobj_vec[best_rect];
      gr_set_fcolor(hud_colors[hud_color_bank][0]);
      ss_box(dat->xl-1,dat->yl-1,dat->xh+1,dat->yh+1);
   }
}


// A "compound use" object is one of those nasty objects
// that used on another object by double clicking on that
// object.

bool is_compound_use_obj(ObjID obj)
{
   if (objs[obj].obclass != CLASS_SMALLSTUFF)
      return false;
   if (objs[obj].subclass == SMALLSTUFF_SUBCLASS_PLOT)
      return true;
   switch(ID2TRIP(obj))
   {
      case EPICK_TRIPLE:
      case HEAD_TRIPLE:
      case HEAD2_TRIPLE:
         return true;
      default:
         break;
   }
   return false;
}

void olh_do_cursor(int16_t xl, int16_t yl)
{
   ObjID obj = object_on_cursor;
   // this should be a different string if the cursor is
   // a live grenade.
   int8_t buf[80];
   if (objs[obj].obclass == CLASS_GRENADE && objGrenades[objs[obj].specID].flags & GREN_ACTIVE_FLAG
      || (ObjProps[OPNUM(obj)].flags & USELESS_FLAG) != 0)
      get_string(REF_STR_helpGrenade,buf,sizeof(buf));
   else
   {
      int8_t stringbuf[80];
      int8_t namebuf[80];
      Ref id = is_compound_use_obj(obj) ? REF_STR_helpCompound : REF_STR_helpCursor;

      get_string(id,stringbuf,sizeof(stringbuf));
      get_object_lookname(obj,namebuf,sizeof(namebuf));
      lg_sprintf(buf,stringbuf,namebuf);
   }
   draw_olh_string(buf,xl,yl);
}

// -------------------------------------------------
// olh_do_hudobjs is called by the hud system to
// draw olh hud.

void olh_do_hudobjs(int16_t xl,int16_t yl)
{
   extern bool saveload_static;
   if (global_fullmap->cyber || saveload_static)
      return;
   if (input_cursor_mode == INPUT_OBJECT_CURSOR)
      olh_do_cursor(xl,yl);
   else if (player_struct.panel_ref != OBJ_NULL)
   {
      int32_t i;
      for (i = 0; i < NUM_MFDS; i++)
         if (player_struct.mfd_current_slots[i] == MFD_INFO_SLOT)
         {
            olh_do_panel_ref(xl,yl);
            return;
         }
   }
   else
      olh_do_callout(xl,yl);
}

/*KLC - no longer used
void olh_init(void)
{
   extern  void olh_init_scan(void);
   olh_init_scan();
}
*/

void olh_closedown(void)
{
   olh_object.obj = OBJ_NULL;
}

void olh_shutdown(void)
{
   extern  void olh_free_scan(void);
   olh_free_scan();
}


int16_t _olh_overlay_keys[] =
{
   ' '|KB_FLAG_DOWN,
   '?'|KB_FLAG_DOWN,
};

#define NUM_OVERLAY_KEYS (sizeof(_olh_overlay_keys)/sizeof(_olh_overlay_keys[0]))

void olh_overlay(void)
{
   extern LGCursor globcursor;
   extern int8_t which_lang;
   bool done = false;

   status_bio_end();
   uiPushGlobalCursor(&globcursor);
   gr_push_canvas(grd_screen_canvas);
   uiHideMouse(NULL);
   draw_hires_resource_bm(REF_IMG_bmHelpOverlayEnglish + MKREF(which_lang,0),0,0);
   uiShowMouse(NULL);
   gr_pop_canvas();
   uiFlush();

   while (!done)
   {
      uint16_t 				key;
      mouse_event		me;

      tight_loop(false);
      if (mouse_next(&me) == OK)
      {
      	if (me.type == MOUSE_LDOWN)
      		done = true;
      }
      if (kb_get_cooked(&key))
      {
         int32_t i;
         for(i = 0; i < NUM_OVERLAY_KEYS; i++)
            if (_olh_overlay_keys[i] == key)
            {
               done = true;
               if (i != 0)
                  hotkey_dispatch(key);
            }
      }
   }
   uiPopGlobalCursor();
   uiFlush();
   olh_overlay_on = false;
   screen_draw();
   status_bio_start();
}

bool toggle_olh_func(int16_t , uint32_t , void* )
{
   if (!olh_active)
   {
      string_message_info(REF_STR_helpOn);
      olh_active = true;
   }
   else
   {
      string_message_info(REF_STR_helpOff);
      olh_active = false;
      ResUnlock(RES_olh_strings);					// KLC - added to free strings.
   }
   gShockPrefs.goOnScreenHelp = olh_active;	// KLC - Yeah, got to update this one too and
   SavePrefs(kPrefsResID);							// KLC - save the prefs out to disk.
   return true;
}


bool olh_overlay_func(int16_t keycode, uint32_t context, void* )
{
   if (global_fullmap->cyber)
   {
      string_message_info(REF_STR_NotAvailCspace);
      return true;
   }
   if (full_game_3d)
   {
      change_mode_func(keycode,context,GAME_LOOP);
   }
   olh_overlay_on = true;
   return true;
}

