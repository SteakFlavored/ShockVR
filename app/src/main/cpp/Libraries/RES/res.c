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
//        Res.C        Resource Manager primary access routines
//        Rex E. Bradford (REX)
//
//        See the doc RESOURCE.DOC for information.
/*
* $Header: r:/prj/lib/src/res/rcs/res.c 1.24 1994/07/15 18:19:33 xemu Exp $
* $Log: res.c $
 * Revision 1.24  1994/07/15  18:19:33  xemu
 * added ResShrinkResDescTable
 *
 * Revision 1.23  1994/05/26  13:51:55  rex
 * Added ResInstallPager(ResDefaultPager) to ResInit()
 *
 * Revision 1.22  1994/02/17  11:24:51  rex
 * Moved most funcs out into other .c files
 *
*/

#include <stdlib.h>
#include <string.h>

#include "res.h"
#include "res_.h"
#include "lzw.h"

// The resource descriptor table
ResDesc *gResDesc; // ptr to array of resource descriptors
Id resDescMax; // max id in res desc
#define DEFAULT_RESMAX 1023 // default max resource id
#define DEFAULT_RESGROW 1024 // grow by blocks of 1024 resources (must be power of 2!)

//    ---------------------------------------------------------
//        INITIALIZATION AND TERMINATION
//    ---------------------------------------------------------
//
//    ResInit() initializes resource manager.

void ResInit()
{
    int32_t i;

    // We must exit cleanly
    atexit(ResTerm);

    // Init LZW system
    LzwInit();

    // Allocate initial resource descriptor table, default size (can't fail)
    resDescMax = DEFAULT_RESMAX;
    gResDesc = (ResDesc *)calloc(DEFAULT_RESMAX + 1, sizeof(ResDesc));
    if (gResDesc == NULL) {
        Warning("ResInit: Can't allocate the global resource descriptor table.");
        exit(1);
    }

    gResDesc[ID_HEAD].prev = 0;
    gResDesc[ID_HEAD].next = ID_TAIL;
    gResDesc[ID_TAIL].prev = ID_HEAD;
    gResDesc[ID_TAIL].next = 0;

    // Clear file descriptor array
    for (i = 0; i <= MAX_RESFILENUM; i++)
        resFile[i].fd = -1;
}

//    ---------------------------------------------------------
//
//    ResTerm() terminates resource manager.

void ResTerm()
{
    int32_t i;

    // Close all open resource files
    for (i = 0; i <= MAX_RESFILENUM; i++) {
        if (resFile[i].fd >= 0)
            ResCloseFile(i);
    }

    // Free up resource descriptor table
    if (gResDesc) {
        free(gResDesc);
        gResDesc = NULL;
        resDescMax = 0;
    }
}

//    ---------------------------------------------------------
//
//    ResGrowResDescTable() grows resource descriptor table to
//    handle a new id.
//
//    This routine is normally called internally, but a client
//    program may call it directly too.
//
//        id = id

void ResGrowResDescTable(Id id)
{
    int32_t newAmt, currAmt;
    ResDesc *growPtr;

    // Calculate size of new table and size of current
    newAmt = (id + DEFAULT_RESGROW) & ~(DEFAULT_RESGROW - 1);
    currAmt = resDescMax + 1;

    // If need to grow, do it, clearing new entries
    if (newAmt > currAmt) {
        growPtr = (ResDesc *)calloc(newAmt, sizeof(ResDesc));
        if (growPtr == NULL) {
            Warning("ResGrowDescTable: CANT GROW DESCRIPTOR TABLE!!!\n");
            return;
        }

        memmove(growPtr, gResDesc, currAmt * sizeof(ResDesc));
        free(gResDesc);
        gResDesc = growPtr;
        resDescMax = newAmt - 1;
    }
}
