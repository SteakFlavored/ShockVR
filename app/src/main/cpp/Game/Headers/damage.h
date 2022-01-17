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
 * $Source: r:/prj/cit/src/inc/RCS/damage.h $
 * $Revision: 1.34 $
 * $Author: minman $
 * $Date: 1994/08/09 04:46:01 $
 *
 */

#ifndef __DAMAGE_H
#define __DAMAGE_H

// Includes
#include "objects.h"
#include "combat.h"

#define EXPLOSION_TYPE              1
#define ENERGY_BEAM_TYPE            2
#define MAGNETIC_TYPE                3
#define RADIATION_TYPE              4

#define GAS_TYPE                      5
#define TRANQ_TYPE                    6
#define NEEDLE_TYPE                  7
#define BIO_TYPE                      8

#define DAMAGE_TYPE2MASK(n) (1 << ((n) - 1))

#define DAMAGE_TYPE_FIELD          0x00FF
#define PRIMARY_DAMAGE_FIELD      0x0F00
#define PRIMARY_DAMAGE(X)          (((X) >> 8) & 0xF)
#define SUPER_DAMAGE_FIELD         0xF000
#define SUPER_DAMAGE(X)             (((X) >> 12) & 0xF)

#define CYBER_EXPLOSION_TYPE      1
#define CYBER_PROJECTILE_TYPE     2
#define CYBER_DRILL_TYPE            3

#define EXPLOSION_FLAG              (0x01 << (EXPLOSION_TYPE-1))
#define ENERGY_BEAM_FLAG            (0x01 << (ENERGY_BEAM_TYPE-1))
#define MAGNETIC_FLAG                (0x01 << (MAGNETIC_TYPE-1))
#define RADIATION_FLAG              (0x01 << (RADIATION_TYPE-1))
#define GAS_FLAG                      (0x01 << (GAS_TYPE-1))
#define TRANQ_FLAG                    (0x01 << (TRANQ_TYPE-1))
#define NEEDLE_FLAG                  (0x01 << (NEEDLE_TYPE-1))
#define SLEEP_FLAG                    (0x01 << (SLEEP_TYPE-1))

#define CYBER_EXPLOSION_FLAG      (0x01 << (CYBER_EXPLOSION_TYPE-1))
#define CYBER_PROJECTILE_FLAG     (0x01 << (CYBER_PROJECTILE_TYPE-1))
#define CYBER_DRILL_FLAG            (0x01 << (CYBER_DRILL_TYPE-1))

#define DAMAGE_MIN      0
#define DAMAGE_MAX      4
#define DAMAGE_DEGREES 6

#define DAMAGE_NONE          0
#define DAMAGE_LIGHT         1
#define DAMAGE_MEDIUM        2
#define DAMAGE_SEVERE        3
#define DAMAGE_CRITICAL     4
#define DAMAGE_INEFFECTIVE 5
#define DAMAGE_TRANQ         6
#define DAMAGE_STUN          7

// attack_object flags
#define NO_SHIELD_ABSORBTION      0x01              // should the player's shield not absorb damage
                                                                // default is to absorb
#define FLASH_BLOOD                  0x02
#define STUN_ATTACK                  0x04

#define MAX_DESTROYED_OBJS     100

#ifdef __COMBAT_C
int16_t destroyed_obj_count = 0;
ObjID destroyed_ids[MAX_DESTROYED_OBJS];
#else
extern int16_t destroyed_obj_count;
extern ObjID destroyed_ids[MAX_DESTROYED_OBJS];
#endif

#define ADD_DESTROYED_OBJECT(X) (destroyed_ids[destroyed_obj_count++] = X)

// is_obj_destroyed()
// returns true, if object with id, is scheduled for destruction
bool is_obj_destroyed(ObjID id);

// destroy_destroyed_objects()
// destroy all objects that have been scheduled to be destroyed
void destroy_destroyed_objects(void);

// damage_object()
// flags - if the low bit is set - then damage is not absorbed by shields
// damage_modifier - raw damage value of attack - 0 if attacking player
uint8_t damage_object(ObjID target_id, int32_t damage, int32_t dtype, uint8_t flags);

int32_t compute_damage(ObjID target,int32_t damage_type,int32_t damage_mod,uint8_t offense,uint8_t penet,int32_t power_level,uint8_t *effect,uint8_t *effect_row, uint8_t attack_effect_type);

uint8_t object_affect(ObjID target_id, int16_t dtype);

// simple_damage object just takes some damage and damage type
// (and flags) and damages the object if it is vulnerable.

bool simple_damage_object(ObjID target, int32_t damage, uint8_t dtype, uint8_t flags);
bool terrain_damage_object(physics_handle ph, fix raw_damage);
bool special_terrain_hit(ObjID cobjid);

// attack_object()
//
// general all purpose attack/damage object
//    target - the ObjID of the target being attacked
//    weapon_triple - the triple for the bullet, grenade, or beam gun
//    flags - see above
//    power_level - will be ignored for projectile weapons, but used for grenades and beam guns
//
uint8_t attack_object(ObjID target, int32_t damage_type,int32_t damage_mod, uint8_t offense, uint8_t penet, uint8_t flags, int32_t power_level, uint8_t *effect_row, uint8_t *effect, uint8_t attack_effect_type, int32_t *damage_inflicted);

// player_attack_object
//
uint8_t player_attack_object(ObjID target, int32_t wpn_triple, int32_t power_level, Combat_Pt origin);

// get an estimate of how damaged we are, with above numerical index
int32_t get_damage_estimate(ObjSpecID osid);

void spew_object_specs(void);
bool test_object_specs(int16_t keycode, uint32_t context, void *data);
bool damage_player(int32_t damage, uint8_t dtype, uint8_t flags);

#endif // __DAMAGE_H
