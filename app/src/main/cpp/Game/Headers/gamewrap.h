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
#ifndef __GAMEWRAP_H
#define __GAMEWRAP_H

/*
 * $Source: n:/project/cit/src/inc/RCS/gamewrap.h $
 * $Revision: 1.13 $
 * $Author: xemu $
 * $Date: 1994/03/24 01:22:08 $
 *
 *
 */

// Includes

// Remember, change in wrapper.c also
#define OLD_SAVE_GAME_ID_BASE 550
#define SAVE_GAME_ID_BASE  4000
#define NUM_RESIDS_PER_LEVEL  100
#define CURRENT_GAME_FNAME "\pCurrent Game"
#define ARCHIVE_FNAME "\parchive.data"

#define ResIdFromLevel(level) (SAVE_GAME_ID_BASE+(level*NUM_RESIDS_PER_LEVEL)+2)

// Defines

// Typedefs

// Prototypes

// Loads or saves a game named by fname.
//KLC - following is Mac version		errtype copy_file(int8_t *src_fname, int8_t *dest_fname);
errtype copy_file(FSSpec *srcFile, FSSpec *destFile, bool saveGameFile);
//KLC - following is Mac version 		errtype save_game(int8_t *fname, int8_t *comment);
errtype save_game(FSSpec *fSpec);
//KLC - following is Mac version 		errtype load_game(int8_t *fname);
errtype load_game(FSSpec *loadSpec);
errtype write_level_to_disk(int32_t idnum, bool flush_mem);
bool create_initial_game_func(int16_t keycode, uint32_t context, void* data);
bool create_level_archive_func(int16_t keycode, uint32_t context, void* data);
errtype load_level_from_file(int32_t level_num);
void startup_game(bool visible);
void closedown_game(bool visible);

// Globals

#endif // __GAMEWRAP_H
