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
#include "map.h"
#include "objects.h"

int8_t request_pathfind(LGPoint source, LGPoint dest, uint8_t dest_z, uint8_t start_z,bool priority);
int8_t next_step_on_path(int8_t path_id, LGPoint *next, int8_t *steps_left);
errtype check_requests(bool priority);
errtype delete_path(int8_t path_id);
bool check_path_cutting(LGPoint new_sq, int8_t path_id);
errtype reset_pathfinding();
int8_t compute_next_step(int8_t path_id, LGPoint *pt, int8_t step_num);
bool pf_check_doors(MapElem *pme, int8_t dir, ObjID *open_door);
bool pf_obj_doors(MapElem *pme1, MapElem *pme2, int8_t dir, ObjID *open_door);

#define NUM_PATH_STEPS      64
#define MAX_PATHS             16

typedef struct {
    LGPoint source;
    LGPoint dest;
    // dest_z and start_z are in objLoc height coordinates
    uint8_t dest_z;
    uint8_t start_z;
    int8_t num_steps;
    int8_t curr_step;
    uint8_t moves[NUM_PATH_STEPS / 4];  // each char holds 4 steps, so we need 16 of 'em
} Path;

#ifdef __PATHFIND_SRC
Path paths[MAX_PATHS];
uint16_t used_paths = 0;
#else
extern Path paths[MAX_PATHS];
extern uint16_t used_paths;
#endif

#define path_length(x)  (paths[(x)].num_steps)
