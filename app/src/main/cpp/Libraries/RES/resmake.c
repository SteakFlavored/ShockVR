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
//        ResMake.c        Resource making
//        Rex E. Bradford
/*
* $Header: n:/project/lib/src/res/rcs/resmake.c 1.2 1994/06/16 11:08:04 rex Exp $
* $Log: resmake.c $
 * Revision 1.2  1994/06/16  11:08:04  rex
 * Modified LRU list handling, lock resource made with ResMake() instead of
 * setting RDF_NODROP flag
 *
 * Revision 1.1  1994/02/17  11:23:57  rex
 * Initial revision
 *
*/

#include <string.h>

#include "res.h"
#include "res_.h"

//    --------------------------------------------------------
//
//    ResMake() makes a resource from a data block.
//
//        Id        = id of resource
//        ptr      = ptr to memory block (resource is not copied; this should
//                    point to storage where the resource can live indefinitely)
//        size     = size of resource in bytes
//        type     = resource type (RTYPE_XXX)
//        filenum = file number
//        flags    = flags (RDF_XXX)
//    --------------------------------------------------------
//  For Mac version, use Resource Manager to add the resource to indicated res file.

void ResMake(Id id, void *ptr, int32_t size, uint8_t type, int32_t filenum, uint8_t flags)
{
    ResDesc *prd;

    ResExtendDesc(id);

    // Check for resource at that id.  If the handle exists, then just change the
    // handle (adjusting for size if needed, of course).
    prd = RESDESC(id);
    if (prd->offset)
        ResDelete(id);

    // Add us to the soup, set lock so doesn't get swapped out
    prd->ptr = ptr;
    prd->offset = RES_OFFSET_PENDING;
    prd->size = size;
    prd->filenum = filenum;
    prd->lock = 1;
    prd->flags = flags;
    prd->type = type;
}

//    -------------------------------------------------------------
//
//    ResUnmake() removes a resource from the LRU list and sets its
//        ptr to NULL.  In this way, a program may take over management
//        of the resource data, and the RES system forgets about it.
//        This is typically done when user-managed data needs to be
//        written to a resource file, using ResMake(),ResWrite(),ResUnmake().
//
//        id = id of resource to unmake
//    --------------------------------------------------------
//  For Mac version: use ReleaseResource to free the handle (the pointer that
//  the handle was made from will still be around).

void ResUnmake(Id id)
{
    memset(RESDESC(id), 0, sizeof(ResDesc));
}

