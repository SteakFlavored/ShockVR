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
 * $Source: n:/project/cit/src/inc/RCS/mapcount.h $
 * $Revision: 1.2 $
 * $Author: dc $
 * $Date: 1994/01/22 18:57:10 $
 */

#ifdef MAP_ACCESS_COUNT
int32_t  me_tiletype(MapElem *me_ptr);
int32_t  me_tmap_flr(MapElem *me_ptr);
int32_t  me_tmap_wall(MapElem *me_ptr);
int32_t  me_tmap_ceil(MapElem *me_ptr);
int32_t  me_tmap(MapElem *me_ptr,int32_t idx);
int32_t  me_objref(MapElem *me_ptr);
int32_t  me_flags(MapElem *me_ptr);
int32_t  me_height_flr(MapElem *me_ptr);
int32_t  me_height_ceil(MapElem *me_ptr);
int32_t  me_param(MapElem *me_ptr);
int32_t  me_height(MapElem *me_ptr,int32_t idx);
int32_t  me_cybcolor_flr(MapElem *me_ptr);
int32_t  me_cybcolor_ceil(MapElem *me_ptr);
int32_t  me_templight_flr(MapElem *me_ptr);
int32_t  me_templight_ceil(MapElem *me_ptr);
int32_t  me_subclip(MapElem *me_ptr);
int32_t  me_clearsolid(MapElem *me_ptr);
int32_t  me_rotflr(MapElem *me_ptr);
int32_t  me_rotceil(MapElem *me_ptr);
int32_t  me_flicker(MapElem *me_ptr);
int32_t  me_quickclip(MapElem *me_ptr);

void me_tiletype_set(MapElem *me_ptr,int32_t v);
void me_tmap_flr_set(MapElem *me_ptr,int32_t v);
void me_tmap_wall_set(MapElem *me_ptr,int32_t v);
void me_tmap_ceil_set(MapElem *me_ptr,int32_t v);
void me_objref_set(MapElem *me_ptr,int32_t v);
void me_flags_set(MapElem *me_ptr,int32_t v);
void me_height_flr_set(MapElem *me_ptr,int32_t v);
void me_height_ceil_set(MapElem *me_ptr,int32_t v);
void me_param_set(MapElem *me_ptr,int32_t v);
void me_height_set(MapElem *me_ptr,int32_t idx,int32_t v);
void me_tmap_set(MapElem *me_ptr,int32_t idx,int32_t v);
void me_cybcolor_flr_set(MapElem *me_ptr,int32_t v);
void me_cybcolor_ceil_set(MapElem *me_ptr,int32_t v);
void me_templight_flr_set(MapElem *me_ptr,int32_t v);
void me_templight_ceil_set(MapElem *me_ptr,int32_t v);
void me_subclip_set(MapElem *me_ptr, int32_t v);
void me_clearsolid_set(MapElem *me_ptr, int32_t v);
void me_rotflr_set(MapElem *me_ptr, int32_t v);
void me_rotceil_set(MapElem *me_ptr, int32_t v);
void me_flicker_set(MapElem *me_ptr, int32_t v);
void me_quickclip_set(MapElem *me_ptr, int32_t v);

int32_t  me_flag1(MapElem *me_ptr);
int32_t  me_flag2(MapElem *me_ptr);
int32_t  me_flag3(MapElem *me_ptr);
int32_t  me_flag4(MapElem *me_ptr);
int32_t  me_rotflr_x(MapElem *me_ptr);
int32_t  me_rotceil_x(MapElem *me_ptr);
int32_t  me_hazard_bio_x(MapElem *me_ptr);
int32_t  me_hazard_bio(MapElem *me_ptr);
int32_t  me_hazard_rad_x(MapElem *me_ptr);
int32_t  me_hazard_rad(MapElem *me_ptr);
int32_t  me_tmap_flr_x(MapElem *me_ptr);
int32_t  me_tmap_ceil_x(MapElem *me_ptr);
int32_t  me_tmap_wall_x(MapElem *me_ptr);
int32_t  me_flicker_x(MapElem *me_ptr);
int32_t  me_quickclip_x(MapElem *me_ptr);
int32_t  me_templight_ceil_x(MapElem *me_ptr);
int32_t  me_templight_flr_x(MapElem *me_ptr);

void me_flag1_set(MapElem *me_ptr, int32_t v);
void me_flag2_set(MapElem *me_ptr, int32_t v);
void me_flag3_set(MapElem *me_ptr, int32_t v);
void me_flag4_set(MapElem *me_ptr, int32_t v);
void me_hazard_bio_set(MapElem *me_ptr, int32_t v);
void me_hazard_rad_set(MapElem *me_ptr, int32_t v);
#endif
