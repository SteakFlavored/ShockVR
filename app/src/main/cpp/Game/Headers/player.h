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
#ifndef __PLAYER_H
#define __PLAYER_H

/*
 * $Source: r:/prj/cit/src/inc/RCS/player.h $
 * $Revision: 1.95 $
 * $Author: minman $
 * $Date: 1994/09/06 21:17:30 $
 *
 *
 */

// Includes
#include "gamesys.h"
#include "objects.h"
#include "map.h"

// Defines
#define DEGREES_OF_FREEDOM 6 // number of physics control axes.

// Basic quantities, of MFD's, slots, buttons, functions
#define NUM_MFDS              2
#define MFD_NUM_VIRTUAL_SLOTS 5
#define MFD_NUM_REAL_SLOTS    (MFD_NUM_VIRTUAL_SLOTS+NUM_MFDS)
#define MFD_NUM_FUNCS         32 // this oughta do for now.

#define NUM_EMAIL_PROPER      47
#define NUM_LOG_LEVELS        14
#define LOGS_PER_LEVEL        16
#define NUM_DATA              23

#define NUM_EMAIL (NUM_EMAIL_PROPER+NUM_DATA+NUM_LOG_LEVELS*LOGS_PER_LEVEL)

#define NUM_DAMAGE_TYPES      8

typedef enum {
   MFD_EMPTY, MFD_FLASH, MFD_ACTIVE, MFD_UNAVAIL
} MFD_Status;

#define DEFAULT_FATIGUE_REGEN 50
#define NUM_WEAPON_SLOTS 7
#define EMPTY_WEAPON_SLOT 0xFF
#define NUM_GENERAL_SLOTS 14

typedef enum {
 ACTIVE_WEAPON        = 0,
 ACTIVE_GRENADE       = 1,
 ACTIVE_DRUG          = 2,
 ACTIVE_CART          = 3,
 ACTIVE_HARDWARE      = 4,
 ACTIVE_COMBAT_SOFT   = 5,
 ACTIVE_DEFENSE_SOFT  = 6,
 ACTIVE_MISC_SOFT     = 7,
 ACTIVE_GENERAL       = 8,
 ACTIVE_EMAIL         = 9,
 NUM_ACTIVES          = 10
} Actives;

#define PLAYER_VERSION_NUMBER    ((int32_t)6)

// Some questbitty stuff
#define NUM_QUESTBITS         512
#define QUESTBIT_GET(qnum)    (player_struct.questbits[((qnum) / 8)] & (1 << ((qnum) % 8)))
#define QUESTBIT_ON(qnum)     (player_struct.questbits[((qnum) / 8)] |= (1 << ((qnum) % 8)))
#define QUESTBIT_OFF(qnum)     (player_struct.questbits[((qnum) / 8)] &= ~(1 << ((qnum) % 8)))

#define NUM_QUESTVARS         64
#define QUESTVAR_GET(qnum)    (player_struct.questvars[(qnum)])
#define QUESTVAR_SET(qnum,x)  do { player_struct.questvars[(qnum)] = (x); } while(0)

#define COMBAT_DIFF_INDEX  0
#define QUEST_DIFF_INDEX   1
#define PUZZLE_DIFF_INDEX  2
#define CYBER_DIFF_INDEX   3

#define PLAYER_MAX_HP      255

#define get_righto_hp(w)  (*((&(player_struct.hit_points))+w))

typedef struct _weapon_slot
{
   uint8_t type;    // type of weapon in slot or EMPTY_WEAPON_SLOT
   uint8_t subtype; // subtype of weapon
   union {
      uint8_t ammo;    // current number of rounds
      uint8_t heat;    // how hot am I?
   };
   union {
      uint8_t ammo_type; // current ammo type.
      uint8_t setting;   // current charge setting.
   };
   uint8_t make_info; // manufacturer
}  weapon_slot;

typedef struct _softs
{
   uint8_t combat[NUM_COMBAT_SOFTS];
   uint8_t defense[NUM_DEFENSE_SOFTS];
   uint8_t misc[NUM_MISC_SOFTS];
}  softs_data;


typedef struct _Player {
   // Static Game Data
   int8_t name[20];
   int8_t realspace_level;  // this is the last realspace level we were in

   // Difficulty related stuff
   int8_t difficulty[4];
   uint8_t level_diff_zorched[(NUM_LEVELS / 8) + 1];    // bitfield for levels -- 1 if difficulty dealt with yet, 0 else

   // system stuff
   uint32_t game_time;
   uint32_t last_second_update; // when was last do_stuff_every_second
   uint32_t last_drug_update;
   uint32_t last_ware_update;
   uint32_t last_anim_check;
   int32_t queue_time;
   int32_t deltat;
   int8_t detail_level;

   // World stuff
   uint8_t level;                     // current level
   int16_t initial_shodan_vals[NUM_LEVELS]; // initial shodan security levels
   int8_t controls[DEGREES_OF_FREEDOM]; // physics controls
   ObjID rep;                       // The player's object-system object
   ObjLoc realspace_loc;            // This is where the player will come back out of cspace into
   int32_t version_num;
   ObjID inventory[NUM_GENERAL_SLOTS];   // general inventory

   // Random physics state.
   uint8_t posture;                   // current posture (standing/stooped/prone)
   bool  foot_planted;              // Player's foot is planted
   int8_t leanx, leany;               // leaning, -100-+100

   // not used - eye postion - lower in struct!!!!
   fixang eye;                      // eye position

   // Gamesys stuff
   uint8_t hit_points;                // I bet we will want these.
   uint8_t cspace_hp;                 // after hit_points so we can array ref this stuff
   uint16_t hit_points_regen;         // Rate at which hit points regenerate, per minute
   uint8_t hit_points_lost[NUM_DAMAGE_TYPES];  // Rate at which damage is taken, per minute
   uint16_t bio_post_expose;          // expose damage from bio squares long past.
   uint16_t rad_post_expose;          // expose damage from rad squares long past.
   uint8_t energy;                    // suit power charge
   uint8_t energy_spend;              // rate of energy burn
   uint8_t energy_regen;              // Rate at which suit recharges
   bool  energy_out;                // out of energy last check
   int16_t cspace_trips;
   int32_t   cspace_time_base;
   uint8_t questbits[NUM_QUESTBITS / 8];       // Mask of which "quests" you have completed
   int16_t questvars[NUM_QUESTVARS];
   uint32_t hud_modes;                 // What hud functions are currently active?
   bool  experience;                // Are you experienced?
   int32_t   fatigue;                   // how fatigued are you
   uint16_t   fatigue_spend;          // Current rate of fatigue expenditure in pts/sec
   uint16_t   fatigue_regen;          // Current rate of fatigue regeneration
   uint16_t   fatigue_regen_base;     // base fatigue regen rate
   uint16_t   fatigue_regen_max;      // max fatigue regen rate
   int8_t  accuracy;
   uint8_t  shield_absorb_rate;       // % of damage shields absorb
   uint8_t   shield_threshold;        // Level where shields turn off
   uint8_t light_value;               // current lamp setting

   // MFD State
   uint8_t      mfd_virtual_slots[NUM_MFDS][MFD_NUM_VIRTUAL_SLOTS]; // ptrs to mfd_slot id's
   MFD_Status mfd_slot_status[MFD_NUM_REAL_SLOTS];
   uint8_t      mfd_all_slots[MFD_NUM_REAL_SLOTS];          // ptrs to mfd_func id's
   uint8_t      mfd_func_status[MFD_NUM_FUNCS];             // ptrs to mfd_func flags
   uint8_t      mfd_func_data[MFD_NUM_FUNCS][8];
   uint8_t      mfd_current_slots[NUM_MFDS];                // ptrs to mfd's curr slots
   uint8_t      mfd_empty_funcs[NUM_MFDS];                  // ptrs to mfd's empty func
   uint8_t      mfd_access_puzzles[64];							// this is 4 times as much as that hardcoded 8 up there
                                                         					 		// who knows how much we really need, hopefully in soon
                                                         					 		// KLC - changed to 64
   int8_t      mfd_save_slot[NUM_MFDS];

   // Inventory stuff, in general, a value of zero will indicate an empty slot
   // indices are drug/grenade/ware "types"
   uint8_t hardwarez[NUM_HARDWAREZ];  // Which warez do we have? (level of each type?)
   softs_data softs;
   uint8_t cartridges[NUM_AMMO_TYPES];// Cartridges for each ammo type.
   uint8_t partial_clip[NUM_AMMO_TYPES];
   uint8_t drugs[NUM_DRUGZ];          // Quantity of each drug
   uint8_t grenades[NUM_GRENADEZ];    // Quantity of each grenade.

   bool email[NUM_EMAIL];  // Which email messages do you have.
   uint8_t logs[NUM_LOG_LEVELS]; // on which levels do we have logs.

   // Weapons are arranged into "slots"
   weapon_slot weapons[NUM_WEAPON_SLOTS]; // Which weapons do you have?

   // Inventory status
   uint8_t hardwarez_status[NUM_HARDWAREZ];    // Status of active wares (on/off, activation time, recharge time?)
   struct _softs softs_status;
   uint8_t jumpjet_energy_fraction;  // fractional units of energy spent on jumpjets.
   uint8_t email_sender_counts[32];  // who has sent how many emails
   int8_t drug_status[NUM_DRUGZ];     // Time left on active drugs, 0 if inactive
   uint8_t drug_intensity[NUM_DRUGZ];  // Intensity of active drugs, 0 if inactive
   uint16_t grenades_time_setting[NUM_GRENADEZ];      // Time setting for each grenade

   // PLOT STUFF
   uint16_t time2dest;  // Time to destination (seconds)
   uint16_t time2comp;  // time to completion of current program (seconds)

   // Combat shtuff <tm>
   ObjID  curr_target;                   // creature currently "targeted"
   uint32_t      last_fire;                 // last gametime the weapon fired.
   uint16_t     fire_rate;                 // game time required between weapon fires.

   // Selectied items
   uint8_t actives[NUM_ACTIVES];

   // Other transitory state
   ObjID save_obj_cursor;            // saving object cursor when you change to cyberspace
   ObjID panel_ref;                 // Last panel utilized.  stuffed here for reference

   // Stats...
   int32_t num_victories;
   int32_t time_in_cspace;
   int32_t rounds_fired;
   int32_t num_hits;

   // Playtesting data
   int32_t num_deaths;

   // from this point on - data is taking the time_to_level space
   int32_t  eye_pos;       // physics eye position

   // let's hope State stays at 12 fixes
   fix   edms_state[12];

   // the player's actively selected inventory category.
   uint8_t current_active;
   uint8_t active_bio_tracks;

   int16_t current_email;

   int8_t version[6];
   bool dead;

   uint16_t lean_filter_state;
   uint16_t FREE_BITS_HERE;
   uint8_t mfd_save_vis;

   uint32_t auto_fire_click;

   uint32_t posture_slam_state;

   bool terseness;

   uint32_t last_bob;         // not last paul, but last bob

   uint8_t pad[9];
} Player;

#define PLAYER_OBJ   (player_struct.rep)
#define PLAYER_BIN_X OBJ_LOC_BIN_X(objs[PLAYER_OBJ].loc)
#define PLAYER_BIN_Y OBJ_LOC_BIN_Y(objs[PLAYER_OBJ].loc)
#define PLAYER_FINE_X OBJ_LOC_FINE_X(objs[PLAYER_OBJ].loc)
#define PLAYER_FINE_Y OBJ_LOC_FINE_Y(objs[PLAYER_OBJ].loc)
#define PLAYER_PHYSICS (objs[PLAYER_OBJ].info.ph)
#define player_physics PLAYER_PHYSICS

// Prototypes
errtype init_player(Player *pplr);
errtype player_tele_to(int32_t x, int32_t y);
errtype player_create_initial(void);
errtype player_startup(void);
errtype player_shutdown(void);
uint8_t set_player_energy_spend(uint8_t new_val);
bool IsFullscreenWareOn(void);

// Globals
#ifdef __PLAYER_SRC
Player player_struct;
Obj *player_dos_obj;
#else
extern Player player_struct;
extern Obj *player_dos_obj;
#endif

#endif // __PLAYER_H

