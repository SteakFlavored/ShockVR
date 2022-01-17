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
 * $Source: r:/prj/cit/src/inc/RCS/minimax.h $
 * $Revision: 1.1 $
 * $Author: tjs $
 * $Date: 1994/09/22 14:18:06 $
 *
 *
 */

void minimax_setup(void* boardpos, uint32_t pos_siz, int8_t depth, bool minimize,
   int32_t (*evaluator)(void*), bool (*generate)(void*,int32_t,bool),
   bool (*horizon)(void*));
void minimax_step(void);
bool minimax_done(void);
void minimax_get_result(int32_t* value, int8_t* which);
void fstack_init(uint8_t* fs, uint32_t siz);
