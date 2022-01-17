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
#include "hudobj.h"
#include "objects.h"
#include "objapp.h"

// -------
// GLOBALS
// -------

uint16_t hudobj_classes[NUM_CLASSES];

struct _hudobj_data hudobj_vec[NUM_HUDOBJS];

uint8_t current_num_hudobjs = 0;


// -------------
// API FUNCTIONS
// -------------

void hudobj_set_subclass(uint8_t obclass, uint8_t subclass, bool val)
{
    uint16_t mask = (subclass == HUDOBJ_ALL_SUBCLASSES) ? 0xFFFF : (1 << subclass);
    if (val) hudobj_classes[obclass] |= mask;
    else hudobj_classes[obclass] &= ~mask;
}

void hudobj_set_id(int16_t id, bool val)
{
    if (id == OBJ_NULL) return;
    if (val) objs[id].info.inst_flags |=  HUDOBJ_INST_FLAG;
    else      objs[id].info.inst_flags &= ~HUDOBJ_INST_FLAG;
}
