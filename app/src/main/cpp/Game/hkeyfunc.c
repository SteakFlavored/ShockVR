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
 * $Source: r:/prj/cit/src/RCS/hkeyfunc.c $
 * $Revision: 1.173 $
 * $Author: dc $
 * $Date: 1994/11/18 00:24:50 $
 */

#define __HKEYFUNC_SRC

#include <string.h>
#include <stdlib.h>

#include "Shock.h"
#include "ShockBitmap.h"
#include "InitMac.h"
#include "Prefs.h"

#include "frtypes.h"
#include "FrUtils.h"
#include "fullscrn.h"
#include "hkeyfunc.h"
#include "loops.h"      // this is needed for demoview update
#include "mfdint.h"
#include "mfdext.h"
#include "MacTune.h"
#include "musicai.h"
#include "screen.h"
#include "status.h"
#include "tools.h"
#include "wares.h"

//--------------
//  PROTOTYPES
//--------------
int32_t select_object_by_class(int32_t obclass, int32_t num, uint8_t* quantlist);


#ifdef NOT_YET //

#include <frprotox.h>
#include <wrapper.h>
#include <saveload.h>
#include <player.h>
#include <physics.h>
#include <fauxrint.h>
#include <frcamera.h>
#include <version.h>
#include <objsim.h>
#include <gamewrap.h>
#include <damage.h>
#include <cutscene.h>
#include <cursors.h>
#include <render.h>
#include <status.h>
#include <scrntext.h>
#include <keydefs.h>
#include <gamescr.h>
#include <gamestrn.h>
#include <shodan.h>
#include <cybmem.h>
#include <gr2ss.h>

#include <cybstrng.h>
#include <memstat.h>

#define SIGNATURE "giSoink"
#define CFG_HKEY_GO "cyberia"
bool yes_3d = true;
extern bool properties_changed;

#ifdef PLAYTEST
#pragma disable_message(202)
bool maim_player(int16_t keycode, uint32_t context, void* data)
{
    player_struct.hit_points = 5;
    return true;
}
#pragma enable_message(202)

#pragma disable_message(202)
bool salt_the_player(int16_t keycode, uint32_t context, void* data)
{
    if (config_get_raw(CFG_HKEY_GO,NULL,0))
    {
        player_struct.hit_points = 255;
        player_struct.cspace_hp = 255;
        memset(player_struct.hit_points_lost,0, NUM_DAMAGE_TYPES);
        player_struct.energy = 255;
        player_struct.fatigue= 0;
        player_struct.experience = true;
        chg_set_flg(VITALS_UPDATE);
    }
    else
    {
        message_info("Winners don't use hotkeys");
        damage_player(25, EXPLOSION_FLAG, 0);
    }
    return true;
}

bool automap_seen(int16_t keycode, uint32_t context, void* data)
{
    uint16_t x,y;
    MapElem* pme;

    if (config_get_raw(CFG_HKEY_GO,NULL,0))
    {
        for(x=0;x<MAP_XSIZE;x++) {
            for(y=0;y<MAP_YSIZE;y++) {
                me_bits_seen_set(MAP_GET_XY(x,y));
            }
        }
    }
    return true;
}

extern errtype give_player_loot(Player *pplr);

bool give_player_hotkey(int16_t keycode, uint32_t context, void *data)
{
    if (config_get_raw(CFG_HKEY_GO,NULL,0))
    {
        give_player_loot(&player_struct);
        chg_set_flg(INVENTORY_UPDATE);
        mfd_force_update();
    }
    return true;
}

#pragma enable_message(202)
#endif

#ifdef PLAYTEST
bool new_cone_clip = true;

#pragma disable_message(202)
bool change_clipper(int16_t keycode, uint32_t context, void *data)
{
    extern errtype render_run(void);
    new_cone_clip = !new_cone_clip;
    if (new_cone_clip)
        mprintf("NEW CONE CLIP\n");
    else
        mprintf("OLD CONE CLIP\n");
    render_run();
    return true;
}
#pragma enable_message(202)
#endif

#pragma disable_message(202)
bool quit_key_func(int16_t keycode, uint32_t context, void* data)
{
#ifndef GAMEONLY
    extern bool possible_change;
#endif

#ifndef GAMEONLY
    if ((!possible_change) || (confirm_box("Level changed without save. Really exit?")))
#endif
    {
        _new_mode = -1;
        chg_set_flg(GL_CHG_LOOP);
    }
    return true;
}

extern void loopmode_exit(int16_t),loopmode_enter(int16_t);

bool keyhelp_hotkey_func(int16_t keycode, uint32_t context, void* data)
{
    void* keyhelp_txtscrn;
    int32_t fake_inp=0;
    extern errtype update_state(bool time_passes);


    loopmode_exit(_current_loop);

    uiHideMouse(NULL);
    uiFlush();

#ifdef RES_keyhelp
    keyhelp_txtscrn=scrntext_init(RES_smallTechFont,0x4C,RES_keyhelp);
    while(scrntext_advance(keyhelp_txtscrn,fake_inp)) {

        fake_inp=0;

        // translate keys to escape
        if(uiCheckInput()) {
            fake_inp=KEY_PGDN;
        }
        tight_loop(false);
    }

    scrntext_free(keyhelp_txtscrn);
#else
    Warning("Ce n'est pas RES_keyhelp\n");
#endif

    uiShowMouse(NULL);

    update_state(false);
    loopmode_enter(_current_loop);

    return true;
}


bool really_quit_key_func(int16_t keycode, uint32_t context, void* data)
{
    _new_mode = -1;
    chg_set_flg(GL_CHG_LOOP);
    return true;
}

bool toggle_bool_func(int16_t keycode, uint32_t context, bool* tgl)
{
    *tgl = !*tgl;
    return true;
}

#endif // NOT_YET

extern bool    DoubleSize;

bool change_mode_func(int16_t , uint32_t , void* data)
{
    int32_t newm = (int32_t)data;

    if ((newm == AUTOMAP_LOOP) && ((!player_struct.hardwarez[HARDWARE_AUTOMAP]) || (global_fullmap->cyber)))
        return true;
    _new_mode = newm;
    chg_set_flg(GL_CHG_LOOP);
    return true;
}

#ifdef NOT_YET //

#ifdef HANDART_ADJUST

int16_t     hdx=0, hdy=0;
uint8_t     hcount = 0;

bool move_handart(int16_t keycode, uint32_t context, void *data)
{
    int16_t amt=1;
    uint8_t foo = (uint8_t) data;
    int16_t *dir;

    if (foo & 0x10)
    {
        hdx=hdy=0;
        return true;
    }

    if (foo & 0x08)
        amt = 10;
    foo &=0x7F;
    dir = (foo&0x02) ? &hdx : &hdy;

    if (foo & 0x01)
        (*dir)+=amt;
    else
        (*dir)-=amt;

    return true;
}

bool adv_handart(int16_t keycode, uint32_t context, void* data)
{
    hcount = (hcount+1)%5;
    return true;
}

#endif // HANDART_ADJUST

bool toggle_view_func(int16_t keycode, uint32_t context, void* data)
{
    extern bool full_game_3d;
    return(change_mode_func(keycode,context, (full_game_3d) ? (void *)GAME_LOOP : (void *)FULLSCREEN_LOOP));
}


#endif // NOT_YET

void start_music(void)
{
    extern errtype mlimbs_AI_init(void);

//    if (music_card)
//    {
    if (MacTuneInit() == 0)
    {
             music_on = true;
        mlimbs_on = true;
        mlimbs_AI_init();
        load_score_for_location(PLAYER_BIN_X, PLAYER_BIN_Y);
        MacTuneStartCurrentTheme();
    }
    else
    {
        gShockPrefs.soBackMusic = false;
        SavePrefs(kPrefsResID);
    }
//    }
}

void stop_music(void)
{
    extern bool mlimbs_on;

    MacTuneShutdown();
    music_on = false;
    mlimbs_on = false;
    mlimbs_peril = DEFAULT_PERIL_MIN;
    mlimbs_monster = NO_MONSTER;
}

bool toggle_music_func(int16_t, uint32_t, void*)
{
    if (music_on)
    {
        message_info("Music off.");
        stop_music();
    }
    else
    {
        start_music();
        if (mlimbs_status == 1)
            message_info("Music on.");
        else
            message_info("Not enough memory to start music.");
    }

    gShockPrefs.soBackMusic = music_on;
    SavePrefs(kPrefsResID);

    return (false);
}


bool arm_grenade_hotkey(int16_t , uint32_t , void* )
{
    extern bool show_all_actives;
    extern void super_drop_func(int32_t dispnum, int32_t row);
    extern void mfd_force_update(void);
    extern errtype inventory_draw(void);
    extern int16_t inv_last_page;
    extern bool activate_grenade_on_cursor(void);
    int32_t i, row, act;

    if(!show_all_actives) {
        show_all_actives=true;
        inv_last_page=-1;
        chg_set_flg(INVENTORY_UPDATE);
        mfd_force_update();
        return true;
    }
    if(activate_grenade_on_cursor())
        return true;
    act=player_struct.actives[ACTIVE_GRENADE];
    for(i=row=0;i<act;i++)
        if(player_struct.grenades[i]) row++;
    super_drop_func(ACTIVE_GRENADE,row);
    return true;
}

int32_t select_object_by_class(int32_t obclass, int32_t num, uint8_t* quantlist)
{
    extern bool show_all_actives;
    extern int16_t inv_last_page;
    int32_t act=player_struct.actives[obclass];
    int32_t newobj=act;

    inv_last_page=-1;
    chg_set_flg(INVENTORY_UPDATE);
    if(!show_all_actives) {
        show_all_actives=true;
        return -1;
    }
    do {
        newobj=(newobj+1)%num;
    } while(quantlist[newobj]==0 && newobj!=act);

    player_struct.actives[obclass]=newobj;
    return newobj;
}

bool select_grenade_hotkey(int16_t , uint32_t , void* )
{
    int32_t newobj;

    newobj=select_object_by_class(ACTIVE_GRENADE,NUM_GRENADES,player_struct.grenades);
    set_inventory_mfd(MFD_INV_GRENADE,newobj,true);
    return true;
}

bool select_drug_hotkey(int16_t , uint32_t , void* )
{
    int32_t newobj;

    newobj=select_object_by_class(ACTIVE_DRUG,NUM_DRUGS,player_struct.drugs);
    set_inventory_mfd(MFD_INV_DRUG,newobj,true);
    return true;
}

bool use_drug_hotkey(int16_t , uint32_t , void* )
{
    extern bool show_all_actives;
    extern void super_use_func(int32_t dispnum, int32_t row);
    extern void mfd_force_update(void);
    extern errtype inventory_draw(void);
    extern int16_t inv_last_page;
    int32_t i, row, act;

    if(!show_all_actives) {
        show_all_actives=true;
        inv_last_page=-1; // to force redraw
        chg_set_flg(INVENTORY_UPDATE);
        return true;
    }
    act=player_struct.actives[ACTIVE_DRUG];
    for(i=row=0;i<act;i++)
        if(player_struct.drugs[i]) row++;
    super_use_func(ACTIVE_DRUG,row);
    return true;
}

bool clear_fullscreen_func(int16_t , uint32_t , void* )
{
    extern int8_t last_message[128];
    extern MFD mfd[2];

    full_lower_region(&mfd[MFD_RIGHT].reg2);
    full_lower_region(&mfd[MFD_LEFT].reg2);
    full_lower_region(inventory_region_full);
    full_visible = 0;
    strcpy(last_message, "");
    chg_unset_sta(FULLSCREEN_UPDATE);
    return(false);
}

#ifdef NOT_YET  //KLC

#ifndef GAMEONLY
bool zoom_func(int16_t keycode, uint32_t context, void* data)
{
    uint16_t zoom;

    TileMapGetZoom(NULL,&zoom);
    if ((int32_t)data == ZOOM_IN)
    {
      zoom++;
    }
    else zoom = (zoom == 1) ? 1 : zoom-1;
    TileMapSetZoom(NULL,zoom);
    return true;
}

bool do_popup_textmenu(int16_t keycode, uint32_t context, void* g)
{
    extern errtype textmenu_popup(Gadget* parent);

    textmenu_popup((Gadget*)g);
    return true;
}
#endif

#define MAP_FNAME "map.dat"

#ifdef PLAYTEST
void edit_load_func(int8_t* fn, uint8_t source, int16_t level_num)
{
    int8_t buf[256], *buf2;
    errtype retval;
    extern Datapath savegame_dpath;
    extern int8_t real_archive_fn[20];
    extern void store_objects(int8_t** buf, ObjID *obj_array, int8_t obj_count);
    extern void restore_objects(int8_t* buf, ObjID *obj_array, int8_t obj_count);
    extern int8_t* get_proj_datadir(int8_t*);

//    if (level_num != 1)
//        player_struct.level = 1;
    if (!strnicmp(fn, "level", 5))
        player_struct.level = atoi(strncpy(buf,fn + 5,2));
    store_objects(&buf2,player_struct.inventory, NUM_GENERAL_SLOTS);
    switch(source)
    {
        case 0:          // local
            strcpy(buf,DATADIR);
            strcat(buf,fn);
            retval = load_current_map(buf,LEVEL_ID_NUM,NULL);
            break;
        case 1:          // currsave
            retval = load_level_from_file(level_num);
            break;
        case 2:          // archive
            retval = load_current_map(real_archive_fn,ResIdFromLevel(level_num),&savegame_dpath);
            break;
        case 3:          // network
            if (get_proj_datadir(buf) != NULL)
            {
                strcat(buf, fn);
                retval = load_current_map(buf,LEVEL_ID_NUM,NULL);
            }
            else retval = ERR_NULL;
            break;
        case 4:          // Old Res
            retval = load_current_map(fn,OLD_LEVEL_ID_NUM,&savegame_dpath);
            break;
    }
    restore_objects(buf2,player_struct.inventory, NUM_GENERAL_SLOTS);
    switch(retval)
    {
        case ERR_FOPEN:
            lg_sprintf(buf,"Error opening %s",fn);
            message_box(buf);
            break;
        case ERR_NOEFFECT:
            message_box("bad map version.");
            break;
        case OK:
            compute_shodometer_value(false);
            config_set_single_value(CFG_LEVEL_VAR,CONFIG_STRING_TYPE,fn);
#ifndef GAMEONLY
            TileMapRedrawPixels(NULL,NULL);
            chg_set_flg(EDITVIEW_UPDATE);
#endif
            message_info("Load complete");
            break;
    }
}
#endif

#ifdef GADGET
bool load_level_func(int16_t keycode, uint32_t context, void* data)
{
    int8_t fn[256];
#ifndef GAMEONLY
    extern bool possible_change;
#endif
    extern Gadget* edit_root_gadget;

    fn[0] = '\0';
#ifndef GAMEONLY
    if ((!possible_change) || (confirm_box("Level changed without save!  Load anyways?")))
#endif
        level_saveload_box("Load Map",_current_root,1,fn,edit_load_func, true);
    return(true);
}
#endif

#define BACKUP_FNAME "shockbak.dat"

#ifdef PLAYTEST
void edit_save_func(int8_t* fn, uint8_t source, int16_t level_num)
{
    int8_t buf[64],b2[64];
    extern void reset_schedules(void);
#ifndef GAMEONLY
    extern bool possible_change;
#endif
    extern int8_t savegame_dir[50];
    extern Datapath savegame_dpath;

    Spew(DSRC_EDITOR_Restore, ("edit_save_func:  fn = %s\n",fn));

    // Make a backup of previous file, if it exists
    if (DatapathFind(&savegame_dpath, fn, buf))
    {
        strcpy(b2, savegame_dir);
        strcat(b2, "\\");
        strcat(b2, BACKUP_FNAME);
        copy_file(buf,b2);
    }
    strcpy(buf,fn);
    reset_schedules();
    switch(save_current_map(buf,LEVEL_ID_NUM, true, true))
    {
        case ERR_FOPEN:
            lg_sprintf(buf,"Error opening %s",fn);
            message_box(buf);
            break;
        case OK:
            message_info("Save complete.");
            config_set_single_value(CFG_LEVEL_VAR,CONFIG_STRING_TYPE,fn);
#ifndef GAMEONLY
            possible_change = false;
#endif
            break;
    }
}
#endif

#ifdef GADGET
bool save_level_func(int16_t keycode, uint32_t context, void* data)
{
    if (!saves_allowed)
    {
        message_box("Saves not allowed -- use control panel to change");
        return(false);
    }
    if ((default_fname == NULL) && !config_get_raw(CFG_LEVEL_VAR,default_fname,256))
        strcpy(default_fname,MAP_FNAME);
    Spew(DSRC_EDITOR_Restore, ("default_fname = %s\n",default_fname));
    level_saveload_box("Save Map",_current_root,1,default_fname,edit_save_func,false);
    return(true);
}
#endif

#ifndef GAMEONLY
bool toggle_3d_func(int16_t keycode, uint32_t context, void* data)
{
    TileEditor* te = (TileEditor*)data;
//    bool yes3d = !chg_get_sta(EDITVIEW_UPDATE);
    Point newsize;
    Point newloc;
    int32_t z;

    if (yes_3d)
    {
        newsize.x = TILEMAP_REGION_WIDTH;
        newsize.y = TILEMAP_REGION_HEIGHT;
        newloc.x = TILEMAP_REGION_X;
        newloc.y = TILEMAP_REGION_Y;
        z = 0;
        chg_set_flg(EDITVIEW_UPDATE);
    }
    else
    {
        newsize.x = VIEW_REGION_WIDTH+TILEMAP_REGION_WIDTH;
        newsize.y = TILEMAP_REGION_HEIGHT;
        z = 1;
        newloc.x = VIEW_REGION_X;
        newloc.y = VIEW_REGION_Y;
        chg_unset_flg(EDITVIEW_UPDATE);
    }
    yes_3d = !yes_3d;
    region_begin_sequence();
    TileEditorResize(te,newsize);
    TileEditorMove(te,newloc,z);
    region_end_sequence(true);
    return true;
}

bool tilemap_mode_func(int16_t keycode, uint32_t context, void* data)
{
    int32_t mode;
    extern errtype terrain_palette_popup(void);
    extern void bitsmode_palette_popup(void);
    extern void cutpaste_palette_popup(void);
    extern void cybpal_popup(void);
    mode = (int32_t)data;
    if (mode != current_palette_mode)
    {
        current_palette_mode = mode;
        switch (mode)
        {
            case OBJECT_MODE:
                object_palette_popup();
                break;
            case TERRAIN_MODE:
                terrain_palette_popup();
                break;
            case EYEBALL_MODE:
                eyeball_palette_popup();
                break;
            case CUTPASTE_MODE:
                cutpaste_palette_popup();
                break;
            case TEXTURING_MODE:
                if (global_fullmap->cyber) cybpal_popup();
                else                              texture_palette_popup();
                break;
            case BITS_MODE:
                bitsmode_palette_popup();
                break;

        }
    }
    return(true);
}

bool draw_mode_func(int16_t keycode, uint32_t context, void* data)
{
    TileEditorSetMode(NULL,(int32_t)data);
    return(true);
}

bool clear_highlight_func(int16_t keycode, uint32_t context, void* data)
{
    TileMapClearHighlights(NULL);
    TileMapRedrawPixels(NULL,NULL);
    return true;
}
#endif

#ifndef GAMEONLY
bool texture_selection_func(int16_t keycode, uint32_t context, void* data)
{
#ifdef TEXTURE_SELECTION
    textpal_create_selector();
#endif
    return(true);
}
#endif

#ifdef GADGET
bool lighting_func(int16_t keycode, uint32_t context, void* data)
{
    panel_create_lighting();
    return(true);
}

bool inp6d_panel_func(int16_t keycode, uint32_t context, void* data)
{
    extern void panel_create_inp6d(void);
    panel_create_inp6d();
    return(true);
}

bool render_panel_func(int16_t keycode, uint32_t context, void* data)
{
    panel_create_renderer();
    return(true);
}

bool popup_tilemap_func(int16_t keycode, uint32_t context, void* data)
{
    return(true);
}

#endif

#ifdef PLAYTEST
bool bkpt_me(int16_t keycode, uint32_t context, void* data)
{  // put a break point here, goof
    return true;
}
#endif

#ifdef GADGET
bool editor_options_func(int16_t keycode, uint32_t context, void* data)
{
    editor_options->parent = _current_root;
    gad_menu_popup_at_mouse(editor_options);
    return(true);
}

bool editor_modes_func(int16_t keycode, uint32_t context, void* data)
{
    editor_modes->parent = _current_root;
    gad_menu_popup_at_mouse(editor_modes);
    return(true);
}

bool misc_menu_func(int16_t keycode, uint32_t context, void* data)
{
    main_misc_menu->parent = _current_root;
    main_misc_menu->parent = _current_root;
    renderer_misc_menu->parent = _current_root;
    gamesys_misc_menu->parent = _current_root;
    editor_misc_menu->parent = _current_root;
    misc_misc_menu->parent = _current_root;
    report_sys_menu->parent = _current_root;
    gad_menu_popup_at_mouse(main_misc_menu);
    return(true);
}

bool control_panel_func(int16_t keycode, uint32_t context, void* data)
{
    panel_create_control();
    return(true);
}
#endif

#ifndef GAMEONLY
bool do_find_func(int16_t keycode, uint32_t context, void* data)
{
    int32_t hilite_num;
    extern errtype generic_tile_eyedropper(TileEditor* te);
    extern errtype TerrainPalUpdate(struct _terrainpal* tp);
    extern void texture_palette_update(void);

//    hilite_num = 0;
    switch (current_palette_mode)
    {
        case OBJECT_MODE:
            TileMapFindHighlightNum(NULL, &hilite_num);
            object_find_func(hilite_num);
            break;
        case TERRAIN_MODE:
            generic_tile_eyedropper(NULL);
            TerrainPalUpdate(NULL);
            break;
        case TEXTURING_MODE:
            generic_tile_eyedropper(NULL);
            texture_palette_update();
            break;

        default:
            generic_tile_eyedropper(NULL);
            break;
    }
    TileMapRedrawSquares(NULL,NULL);
    return(true);
}
#endif

#ifdef PLAYTEST
#ifndef GAMEONLY
bool inp6d_kbd=true;
#else
bool inp6d_kbd=false;
#endif

bool stupid_slew_func(int16_t keycode, uint32_t context, void* data)
{
    int32_t dir = (int32_t)data;
    int32_t v1,v2;
    static int32_t slew_scale=16;
    extern bool inp6d_kbd;

    if (inp6d_kbd==false) return true;

    switch(dir)
    {
        case 1:  v1 = EYE_Y; v2 = slew_scale; break;
        case 2:  v1 = EYE_H; v2 = -slew_scale; break;
        case 3:  v1 = EYE_Y; v2 = -slew_scale; break;
        case 4:  v1 = EYE_H; v2 = slew_scale; break;
        case 5:  v1 = EYE_Z; v2 = slew_scale; break;
        case 6:  v1 = EYE_Z; v2 = -slew_scale; break;
        case 7:  v1 = EYE_P; v2 = -slew_scale; break;
        case 8:  v1 = EYE_P; v2 = slew_scale; break;
        case 9:  v1 = EYE_B; v2 = -slew_scale; break;
        case 10:  v1 = EYE_B; v2 = slew_scale; break;
        case 11:  v1 = EYE_X; v2 = slew_scale; break;
        case 12:  v1 = EYE_X; v2 = -slew_scale; break;
        case 13:  v1 = EYE_RESET; v2 = -slew_scale; break;
        case 14:  if (slew_scale<256) slew_scale<<=1; return true;
        case 15:  if (slew_scale>1) slew_scale>>=1; return true;
    }
    fr_camera_slewcam(NULL,v1,v2);
    if (_current_loop <= FULLSCREEN_LOOP)
        chg_set_flg(DEMOVIEW_UPDATE);
#ifndef GAMEONLY
    if (_current_loop == EDIT_LOOP)
    {
        TileMapUpdateCameras(NULL);
        chg_set_flg(EDITVIEW_UPDATE);
    }
#endif
    return(true);
}

bool zoom_3d_func(int16_t keycode, uint32_t context, void* data)
{
    bool zoomin = (bool)data;

    // cant this be current based?
    if (zoomin)
        fr_mod_cams(_current_fr_context,FR_NOCAM,fix_make(0,62500));
    else
        fr_mod_cams(_current_fr_context,FR_NOCAM,fix_make(1,3000));
    return(true);
}
#endif

#ifdef GADGET
bool menu_close_func(int16_t keycode, uint32_t context, void* data)
{
    return(menu_all_popdown());
}
#endif

#ifdef PLAYTEST
bool mono_clear_func(int16_t keycode, uint32_t context, void* data)
{
    mono_clear();
    return(false);
}

bool mono_toggle_func(int16_t keycode, uint32_t context, void* data)
{
    mono_setmode(MONO_TOG);
    message_info("Monochrome Toggled.");
    return(false);
}
#endif

#ifdef GADGET
Gadget *edit_flags_gadget = NULL;
bool f0, f1, f2;

bool edit_flags_close(void *vg, void *ud)
{
    // Postprocess results into change flags
    if (f0)
    {
        chg_set_sta(ML_CHG_BASE<<0);
        chg_set_flg(ML_CHG_BASE<<0);
    }
    else
    {
        chg_unset_sta(ML_CHG_BASE<<0);
        chg_unset_flg(ML_CHG_BASE<<0);
    }
    if (f1)
    {
        chg_set_sta(ML_CHG_BASE<<1);
        chg_set_flg(ML_CHG_BASE<<1);
    }
    else
    {
        chg_unset_sta(ML_CHG_BASE<<1);
        chg_unset_flg(ML_CHG_BASE<<1);
    }
    if (f2)
    {
        chg_set_sta(ML_CHG_BASE<<2);
        chg_set_flg(ML_CHG_BASE<<2);
    }
    else
    {
        chg_unset_sta(ML_CHG_BASE<<2);
        chg_unset_flg(ML_CHG_BASE<<2);
    }
    gadget_destroy(&edit_flags_gadget);
    return(false);
}

bool edit_flags_func(int16_t keycode, uint32_t context, void* data)
{
    Point pt, ss;

    pt.x = 20; pt.y = 25;
    ss.x = 110;  ss.y = 8;
    if (edit_flags_gadget == NULL)
    {
        f0 = ((_change_flag&(ML_CHG_BASE<<0)) != 0);
        f1 = ((_change_flag&(ML_CHG_BASE<<1)) != 0);
        f2 = ((_change_flag&(ML_CHG_BASE<<2)) != 0);
        edit_flags_gadget = gad_qbox_start(_current_root, pt, 10, &EditorStyle, QB_ALIGNMENT, "edit_flags_gadget", ss);
        gad_qbox_add("Main Loop Flags", QB_TEXT_SLOT, NULL, QB_RD_ONLY);
        gad_qbox_add("Flag 0", QB_BOOL_SLOT, &f0, QB_ARROWS);
        gad_qbox_add("Flag 1", QB_BOOL_SLOT, &f1, QB_ARROWS);
        gad_qbox_add("Frame Rate", QB_BOOL_SLOT, &f2, QB_ARROWS);
        gad_qbox_add("Close", QB_PUSHBUTTON_SLOT, edit_flags_close, QB_NO_OPTION);
        gad_qbox_end();
    }
    return(false);
}

bool music_ai_params_func(int16_t keycode, uint32_t context, void* data)
{
    panel_ai_param_create();
    return(false);
}
#endif

bool version_spew_func(int16_t keycode, uint32_t context, void* data)
{
    int8_t tmpstr[]=SIGNATURE;             /* for tracking versions */
    int8_t temp[40];
    strcpy(temp,".... ");
    if (start_mem >= BIG_CACHE_THRESHOLD)
        temp[0] = 'C';
    if (start_mem > EXTRA_TMAP_THRESHOLD)
        temp[1] = 'T';
    if (start_mem > BLEND_THRESHOLD)
        temp[2] = 'B';
    if (start_mem > BIG_HACKCAM_THRESHOLD)
        temp[3] = 'M';
    strcat(temp,SYSTEM_SHOCK_VERSION);
    message_info(temp);
    return(false);
}

#endif // NOT_YET

int8_t conv_hex(int8_t val);
bool location_spew_func(int16_t , uint32_t , void* );


int8_t conv_hex(int8_t val)
{
    int8_t retval = '?';
    if ((val >= 0) && (val <= 9))
        retval = '0' + val;
    else if ((val >= 10) && (val <= 15))
        retval = 'a' + (val - 10);
    return(retval);
}
/*KLC    moved to TOOLS.C
int32_t str_to_hex(int8_t val)
{
    int32_t retval = 0;
    if ((val >= '0') && (val <= '9'))
        retval = val - '0';
    else if ((val >= 'A') && (val <= 'F'))
        retval = 10 + val - 'A';
    else if ((val >= 'a') && (val <= 'f'))
        retval = 10 + val - 'a';
    return(retval);
}

bool location_spew_func(int16_t , uint32_t , void* )
{
    int8_t goofy_string[32];

//#ifdef SVGA_SUPPORT
//    sprintf(goofy_string,"00:00.00:%s",get_temp_string(REF_STR_ScreenModeText + convert_use_mode));
//#else
    strcpy(goofy_string,"00:00.00 ");
//#endif
    goofy_string[0] = conv_hex( player_struct.level / 16 );
    goofy_string[1] = conv_hex( player_struct.level % 16 );
    if (!time_passes)
        goofy_string[2] = '!';
    goofy_string[3] = conv_hex( PLAYER_BIN_X / 16 );
    goofy_string[4] = conv_hex( PLAYER_BIN_X % 16 );
    if (!physics_running)
        goofy_string[5] = '*';
    goofy_string[6] = conv_hex( PLAYER_BIN_Y / 16 );
    goofy_string[7] = conv_hex( PLAYER_BIN_Y % 16 );

    message_info(goofy_string);
    return(false);
}
*/

#ifdef NOT_YET    //

#ifdef PLAYTEST
bool toggle_physics_func(int16_t keycode, uint32_t context, void* data)
{
    physics_running = !physics_running;
    if (physics_running)
        message_info("Physics turned on");
    else
        message_info("Physics turned off");
    return(false);
}

#define camera_info message_info
bool reset_camera_func(int16_t keycode, uint32_t context, void* data)
{
    extern uint8_t cam_mode;
    extern cams objmode_cam, *motion_cam, player_cam;

    if ((uint8_t *)data)
    {
        if (cam_mode!=OBJ_STATIC_CAMERA) { camera_info("cant toggle"); return false; }
        if (motion_cam!=NULL)
        {
            motion_cam=NULL;
            camera_info("back to cam control");
        }
        else
        {
            motion_cam=fr_camera_getdef();
            camera_info("back to obj control");
        }
    }
    else
    {
        camera_info("camera reset");
        cam_mode = OBJ_PLAYER_CAMERA;
        fr_camera_setdef(&player_cam);
    }
    chg_set_flg(_current_3d_flag);
    return(false);
}

bool current_camera_func(int16_t keycode, uint32_t context, void* data)
{
    extern cams objmode_cam, *motion_cam;
    extern uint8_t cam_mode;
    fix cam_locs[6], *cam_ptr_hack;

    motion_cam=NULL;
    // Not sure what to pass for last two params....
    switch ((uint8_t)data)
    {
    case OBJ_STATIC_CAMERA:
        if (cam_mode==OBJ_DYNAMIC_CAMERA) { camera_info("cant go static"); return false; }
        motion_cam=fr_camera_getdef();            // note super sneaky fall through hack
        camera_info("camera static");
    case OBJ_DYNAMIC_CAMERA:
        fr_camera_modtype(&objmode_cam,CAMTYPE_ABS,CAMBIT_OBJ);
        cam_ptr_hack=fr_camera_getpos(NULL);
        memcpy(cam_locs,cam_ptr_hack,6*sizeof(fix));
        fr_camera_update(&objmode_cam,cam_locs,CAM_UPDATE_NONE,NULL);
        if (motion_cam==NULL) camera_info("camera dynamic");
        break;
    case OBJ_CURRENT_CAMERA:
        camera_info("current obj");
        fr_camera_modtype(&objmode_cam,CAMTYPE_OBJ,CAMBIT_OBJ);
        fr_camera_update(&objmode_cam, (void *)current_object,CAM_UPDATE_NONE,NULL);
        break;
    }
    cam_mode  = (uint8_t)data;
    fr_camera_setdef(&objmode_cam);
    chg_set_flg(_current_3d_flag);
    return(false);
}

bool mono_log_on = false;

bool log_mono_func(int16_t keycode, uint32_t context, void* data)
{
    if (mono_log_on)
    {
        mono_logoff();
        message_info("Mono logging off.");
        mono_log_on = false;
    }
    else
    {
        mono_logon("monolog.txt", MONO_LOG_NEW, MONO_LOG_ALLWIN);
        message_info("Mono logging on.");
        mono_log_on = true;
    }
    return(false);
}

bool clear_transient_lighting_func(int16_t keycode, uint32_t context, void* data)
{
    int32_t x,y;
    MapElem *pme;
    for (x=0; x < MAP_XSIZE; x++)
    {
        for (y=0; y < MAP_YSIZE; y++)
        {
            pme = MAP_GET_XY(x,y);
            me_templight_flr_set(pme,0);
            me_templight_ceil_set(pme,0);
        }
    }
    message_info("Trans. light cleared");
    return(false);
}

bool level_entry_trigger_func(int16_t keycode, uint32_t context, void* data)
{
    extern errtype do_level_entry_triggers();
    do_level_entry_triggers();
    message_info("Level entry triggered.");
    return(false);
}

bool convert_one_level_func(int16_t keycode, uint32_t context, void* data)
{
    extern errtype obj_level_munge();
#ifdef TEXTURE_CRUNCH_HACK
    extern errtype texture_crunch_init();

    texture_crunch_init();
#endif
    obj_level_munge();
    return(true);
}

//#define CONVERT_FROM_OLD_RESID
//#define TEXTURE_CRUNCH_HACK

#define NUM_CONVERT_LEVELS 16

bool convert_all_levels_func(int16_t keycode, uint32_t context, void* data)
{
    int32_t i;
    int8_t atoi_buf[10], fn[10], curr_fname[40], new_fname[40];
    errtype retval;

    extern Datapath savegame_dpath;
    extern void edit_load_func(int8_t* fn, uint8_t source, int16_t level_num);
    extern void edit_save_func(int8_t* fn, uint8_t source, int16_t level_num);
    extern errtype obj_level_munge();
#ifdef TEXTURE_CRUNCH_HACK
    extern errtype texture_crunch_init();

    texture_crunch_init();
#endif

    // save off old level
    edit_save_func("templevl.dat", 0,0);

    // loop through the real levels
    for (i=0; i<NUM_CONVERT_LEVELS; i++)
    {
        retval = OK;
        // load level i
        strcpy(fn, "level");
        strcat(fn, itoa(i,atoi_buf,10));
        strcat(fn, ".dat");
        Spew(DSRC_EDITOR_Modify, ("fn = %s\n",fn));
        if (DatapathFind(&savegame_dpath, fn, curr_fname))
        {
#ifdef CONVERT_FROM_OLD_RESID
            retval = load_current_map(curr_fname, OLD_LEVEL_ID_NUM, &savegame_dpath);
#else
            retval = load_current_map(curr_fname, LEVEL_ID_NUM, &savegame_dpath);
#endif
            Spew(DSRC_EDITOR_Modify, ("convert_all trying to load %s\n",curr_fname));
        }
        else
            retval = ERR_FOPEN;

        Spew(DSRC_EDITOR_Modify, ("curr_fname = %s\n",curr_fname));
        if (retval != OK)
        {
            strcpy(new_fname, "R:\\prj\\cit\\src\\data\\");
            strcat(new_fname, fn);
            retval = load_current_map(new_fname, LEVEL_ID_NUM, NULL);
            Spew(DSRC_EDITOR_Modify, ("new_fname = %s\n",new_fname));
        }

        // Generate the report
        obj_level_munge();
        Spew(DSRC_EDITOR_Modify, ("convert_all trying to save %s\n",fn));
        save_current_map(fn, LEVEL_ID_NUM,true,true);
    }

    // reload original level
    edit_load_func("templevl.dat",0,0);

    return(false);
}

bool invulnerable_func(int16_t keycode, uint32_t context, void* data)
{
    if (config_get_raw(CFG_HKEY_GO,NULL,0))
    {
        player_invulnerable = !player_invulnerable;
        if (player_invulnerable)
            message_info ("invulnerability on");
        else
            message_info ("invulnerability off");
    }
    else
    {
        message_info("Winners don't use hotkeys");
        damage_player(50, EXPLOSION_FLAG, 0);
    }
    return(false);
}

bool pacifist_func(int16_t keycode, uint32_t context, void* data)
{
    extern bool pacifism_on;
    pacifism_on = !pacifism_on;
    if (pacifism_on)
        message_info ("pacifism on");
    else
        message_info ("pacifism off");
    return(false);
}

#endif

int32_t pause_id;
bool remove_pause_handler = false;

bool pause_callback(uiEvent *, LGRegion *, void *)
{
    return(true);
}

bool unpause_callback(uiEvent *, LGRegion *, void *)
{
    return(true);
}

#endif // NOT_YET

bool pause_game_func(int16_t, uint32_t, void*)
{
    extern bool game_paused, redraw_paused;
    extern LGRegion *inventory_region;

    game_paused = true;
    redraw_paused=true;
/* KLC - not needed for Mac version
    game_paused = !game_paused;
    if (game_paused)
    {
        uiPushGlobalCursor(&globcursor);
        uiInstallRegionHandler(inventory_region, UI_EVENT_MOUSE_MOVE, pause_callback, NULL, &pause_id);
        uiGrabFocus(inventory_region, UI_EVENT_MOUSE_MOVE);
        stop_digi_fx();
        redraw_paused=true;
    }
    else
    {
        uiRemoveRegionHandler(inventory_region, pause_id);
        uiReleaseFocus(inventory_region, UI_EVENT_MOUSE_MOVE);
        uiPopGlobalCursor();
    }
*/
    return(false);
}

/*KLC - not needed for Mac version
bool unpause_game_func(int16_t, uint32_t, void*)
{
    extern bool game_paused;
    extern LGRegion *inventory_region;

    if (game_paused)
    {
        game_paused = !game_paused;
        uiRemoveRegionHandler(inventory_region, pause_id);
        uiReleaseFocus(inventory_region, UI_EVENT_MOUSE_MOVE|UI_EVENT_JOY);
        uiPopGlobalCursor();
    }
    return(false);
}
*/

//--------------------------------------------------------------------
//  For Mac version.  Save the current game.
//--------------------------------------------------------------------
bool save_hotkey_func(int16_t, uint32_t, void *)
{
    if (global_fullmap->cyber)                    // Can't save in cyberspace.
    {
        message_info("Can't save game in cyberspace.");
        return true;
    }

    if (music_on)                                    // Setup the environment for doing Mac stuff.
        MacTuneKillCurrentTheme();
    uiHideMouse(NULL);
    ShowCursor();
    CopyBits(&gMainWindow->portBits, &gMainOffScreen.bits->portBits, &gActiveArea, &gOffActiveArea, srcCopy, 0L);

    if (gIsNewGame)                                    // Do the save thang.
    {
        status_bio_end();
        ShowMenuBar();
         DoSaveGameAs();
        HideMenuBar();
        status_bio_start();
    }
    else
         DoSaveGame();

    HideCursor();                                    // Go back to Shock.
    uiShowMouse(NULL);
    if (music_on)
        MacTuneStartCurrentTheme();

    return true;
}


#ifdef NOT_YET  //


//#define CHECK_STATE_N_HOTKEY
#ifdef PLAYTEST
bool check_state_func(int16_t keycode, uint32_t context, void* data)
{
    int32_t avail_memory(int32_t debug_src);
    avail_memory(DSRC_TESTING_Test3);
#ifdef CHECK_STATE_N_HOTKEY
    extern void check_state_every_n_seconds();
    check_state_every_n_seconds();
#endif
#ifdef CORVIN_ZILM_HKEY
    extern bool CorvinZilm;
    extern int32_t watchcount;
    MemStat pms;
    watchcount = 0;
    CorvinZilm = true;
    MemStats(&pms);
#endif
    return(true);
}

bool diffdump_game_func(int16_t keycode, uint32_t context, void* data)
{
    int8_t goof[45];
    sprintf(goof, "diff=%d,%d,%d,%d\n",player_struct.difficulty[0],player_struct.difficulty[1],player_struct.difficulty[2],
        player_struct.difficulty[3]);
    message_info(goof);
    return(true);
}

bool toggle_difficulty_func(int16_t keycode, uint32_t context, void *data)
{
    uint8_t which = (uint8_t) data-1;

    player_struct.difficulty[which]++;
    player_struct.difficulty[which] %= 4;
    return (true);
}

bool toggle_ai_func(int16_t keycode, uint32_t context, void* data)
{
    extern bool ai_on;
    ai_on = !ai_on;
    if (ai_on)
        message_info("AI state on\n");
    else
        message_info("AI state off\n");
    return(true);
}

bool toggle_safety_net_func(int16_t keycode, uint32_t context, void* data)
{
    extern bool safety_net_on;
    safety_net_on = !safety_net_on;
    if (safety_net_on)
        message_info("Safety Net on\n");
    else
        message_info("Safety Net off\n");
    return(true);
}
#endif

#ifdef NEW_RES_LIB_INSTALLED
bool res_cache_usage_func(int16_t keycode, uint32_t context, void* data)
{
    extern int32_t ResViewCache(bool only_locks);
    ResViewCache((bool)data);
    return(true);
}
#endif

#pragma enable_message(202)

#endif // NOT_YET
