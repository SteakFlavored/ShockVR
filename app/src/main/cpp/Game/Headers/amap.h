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
#ifndef __AMAP_H
#define __AMAP_H

// header for the real infernal automap

// defines
#define AMAP_PURE_MODE 0x0000
#define AMAP_INT_WALLS 0x0001
#define AMAP_SHOW_CRIT 0x0002
#define AMAP_SHOW_ROB  0x0004
#define AMAP_SHOW_HAZ  0x0008
#define AMAP_SHOW_FLR  0x0010
#define AMAP_SHOW_MSG  0x0020
#define AMAP_SHOW_HGT  0x0040
#define AMAP_TRACK_OBJ 0x0080
#define AMAP_SHOW_SEC  0x0100
#define AMAP_FULL_MSG  0x0200
#define AMAP_SHOW_ALL  0x0400
#define AMAP_SHOW_SENS 0x0800

#define AMAP_AVAIL_ALWAYS (AMAP_SHOW_SENS|AMAP_SHOW_FLR|AMAP_FULL_MSG)

#define AMAP_SET         1
#define AMAP_UNSET      0
#define AMAP_TOGGLE    -1

#define AMAP_PAN_N      1
#define AMAP_PAN_E      2
#define AMAP_PAN_S      3
#define AMAP_PAN_W      4
#define AMAP_DEF_DST    0x40000

#define AMAP_MAX_ZOOM  6
#define AMAP_MIN_ZOOM  1

#define AMAP_OFF_MAP    0
#define AMAP_HAVE_NOTE 1
#define AMAP_NO_NOTE    2

#define AMAP_NOTE_HACK_PTR ((void *)0xffffffff)

// really should live in the player structure....
typedef struct {
    bool    init;
    uint8_t  zoom;
    int32_t     xf, yf;
    uint16_t lw, lh;
    uint16_t obj_to_follow, sensor_obj;
    uint16_t note_obj;
    uint16_t flags;
    uint16_t avail_flags;
    uint8_t  version_id;
    uint16_t sensor_rad;    // in obj coords
} curAMap;

// prototypes
bool amap_kb_callback(curAMap *amptr, int32_t code);
void amap_draw(curAMap *amptr, int32_t expose);
void amap_version_set(int32_t id, int32_t new_ver);
void automap_init(int32_t version, int32_t id);
void amap_invalidate(int32_t id);
void *amap_loc_note_check(curAMap *amptr, int32_t *x, int32_t *y, int32_t *to_do); // note the void is really a MapElem
bool amap_flags(curAMap *amptr, int32_t flags, int32_t set);                      // set -1 to toggle
bool amap_zoom(curAMap *amptr, bool set, int32_t zoom_delta);
void amap_pan(curAMap *amptr, int32_t dir, int32_t* dist);
bool amap_get_note(curAMap *amptr, int8_t *buf);
void amap_settings_copy(curAMap* from, curAMap* to);

// this is a mess
// it modifies x and y to be map location of click
// returns null if off map, (void*)objid if found, (void*)mapelemptr if empty
void *amap_deal_with_map_click(curAMap *amptr, int32_t *x, int32_t *y);

// strings
void amap_str_init(void);
int8_t *amap_str_next(void);
void amap_str_grab(int8_t *str);
int32_t amap_str_deref(int8_t *str);
int8_t *amap_str_reref(int32_t offs);
void amap_str_delete(int8_t *toast_str);
void amap_str_startup(int32_t magic_num);

#define MFD_FULLSCR_MAP 2
#define NUM_O_AMAP        MFD_FULLSCR_MAP+1

// globals
// for now

#define oAMap(mid) (&(level_gamedata.auto_maps[mid]))
//#define oAMap(mid) (auto_maps[mid])

#define amap_reset() \
do { int32_t i; for(i=0;i<NUM_O_AMAP;i++) amap_invalidate(i); } while(0)

#define amap_note_value(objid)  (objTraps[objs[objid].specID].p4)
#define amap_note_string(objid) (amap_str_reref(amap_note_value(objid)))

#define AMAP_STRING_SIZE    2048

#endif

