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
//        ResAcc.c        Resource access
//        Rex E. Bradford
/*
* $Header: r:/prj/lib/src/res/rcs/resacc.c 1.4 1994/08/30 15:18:20 rex Exp $
* $Log: resacc.c $
 * Revision 1.4  1994/08/30  15:18:20  rex
 * Made sure ResGet() returns NULL if ResLoadResource() did
 *
 * Revision 1.3  1994/08/30  15:14:32  rex
 * Put in check for NULL return from ResLoadResource
 *
 * Revision 1.2  1994/06/16  11:06:05  rex
 * Modified routines to handle LRU list better (keep locked and nodrop stuff out)
 *
 * Revision 1.1  1994/02/17  11:23:31  rex
 * Initial revision
 *
*/

#include <string.h>

#include "res.h"
#include "res_.h"

//    ---------------------------------------------------------
//
//    ResLock() locks a resource and returns ptr.
//
//        id = resource id
//
//    Returns: ptr to locked resource
//    ---------------------------------------------------------

void *ResLock(Id id)
{
    ResDesc *prd;

    // Check if valid id
    if (!ResCheckId(id))
        return NULL;

    // If resource not loaded, load it
    prd = RESDESC(id);
    if (prd->ptr == NULL) {
        if (ResLoadResource(id) == NULL)
            return NULL;
    }

    if (prd->lock == 0)
        ResRemoveFromLRU(prd);

    // Increment lock count, check for overlock
    if (prd->lock < RES_MAXLOCK)
        prd->lock++;

    return prd->ptr;
}

//    ---------------------------------------------------------
//
//    ResUnlock() unlocks a resource.
//
//        id = resource id
//    ---------------------------------------------------------

void ResUnlock(Id id)
{
    ResDesc *prd;

    // Check if valid id
    if (!ResCheckId(id))
        return;

    prd = RESDESC(id);

    //    Check for under-lock
    if (prd->lock == 0) {
        Warning("ResUnlock: id $%x already unlocked\n", id);
        return;
    }

    //    Else decrement lock, if 0 move to tail
    if (prd->lock > 0)
        prd->lock--;

    if (prd->lock == 0)
        ResAddToTail(prd);
}

//    -------------------------------------------------------------
//
//    ResGet() gets a ptr to a resource
//
//        id = resource id
//
//    Returns: ptr to resource (ptr only guaranteed until next Malloc(),
//                Lock(), Get(), etc.
//    ---------------------------------------------------------
//  For Mac version:  Change 'ptr' refs to 'hdl', lock resource handle and return ptr.

void *ResGet(Id id)
{
    ResDesc *prd;

    // Check if valid id
    if (!ResCheckId(id))
        return NULL;

    prd = RESDESC(id);

    // Load resource or move to tail
    if (prd->ptr == NULL) {
        if (ResLoadResource(id) == NULL)
            return NULL;
        ResAddToTail(prd);
    } else if (prd->lock == 0)
        ResMoveToTail(prd);

    // Return ptr
    return prd->ptr;
}


//    ---------------------------------------------------------
//
//    ResExtract() extracts a resource from an open resource file.
//
//        id    = id
//        buff = ptr to buffer
//
//    Returns: ptr to supplied buffer, or NULL if problem
//    ---------------------------------------------------------

void *ResExtract(Id id, void *buffer)
{
    // Retrieve the data into the buffer, please
    if (ResRetrieve(id, buffer))
        return buffer;

    // If ResRetreive failed, return NULL ptr
    return NULL;
}


//    ----------------------------------------------------------
//
//    ResDrop() drops a resource from memory for awhile.
//
//        id = resource id
//    ----------------------------------------------------------
//  For Mac version:  Calls Resource Mgr function EmptyHandle to purge the handle.

void ResDrop(Id id)
{
    ResDesc *prd;

    // Check for locked
    if (!ResCheckId(id))
        return;

    prd = RESDESC(id);
    if (prd->lock != 0)
        Warning("ResDrop: Block $%x is locked, dropping anyway\n", id);

    // Remove from LRU chain
    if (prd->lock == 0)
        ResRemoveFromLRU(prd);

    // Free memory and set ptr to NULL
    if (prd->ptr != NULL) {
        free(prd->ptr);
        prd->ptr = NULL;
    }
}

//    -------------------------------------------------------
//
//    ResDelete() deletes a resource forever.
//
//        Id = id of resource
//    -------------------------------------------------------
//  For Mac version:  Call ReleaseResource on the handle and set its ref to null.
//  The next ResLoadResource on the resource will load it back in.

void ResDelete(Id id)
{
    ResDesc *prd;

    // If locked, issue warning
    if (!ResCheckId(id))
        return;

    prd = RESDESC(id);
    if (prd->lock)
        Warning("ResDelete: Block $%x is locked!\n", id);

    // If in use: if in ram, free memory & LRU, then in any case zap entry
    if (prd->offset) {
        if (prd->ptr != NULL)
            ResDrop(id);

        memset(prd, 0, sizeof(ResDesc));
    }
}

//    --------------------------------------------------------
//        INTERNAL ROUTINES
//    --------------------------------------------------------
//
//    ResCheckId() checks if id valid.
//
//        id = id to be checked
//
//    Returns: true if id ok, false if invalid & prints warning

bool ResCheckId(Id id)
{
    if (id < ID_MIN) {
        Warning("ResCheckId: id $%x invalid\n", id);
        return false;
    } else if (id > resDescMax) {
        Warning("ResCheckId: id $%x exceeds table\n", id);
        return false;
    }

    return true;
}
