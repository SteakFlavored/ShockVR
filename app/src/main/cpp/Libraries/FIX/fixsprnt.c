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
** fixsprnt.c    -    sprintf() routines for fixed-point numbers.
**                        you may want to use %f and %F in lg_sprintf() instead!
**
** $Header: n:/project/lib/src/fix/RCS/fixsprnt.c 1.1 1993/11/04 11:06:24 rex Exp $
** $Log: fixsprnt.c $
 * Revision 1.1  1993/11/04  11:06:24  rex
 * Initial revision
 *
*/

#include <stdlib.h>
#include <stdio.h>

#include "fix.h"

//////////////////////////////
//
// Prints out x nicely into str
// and now returns the string too
//
char *fix_sprint(char *str, fix x)
{
    uint32_t tmp;
    bool neg = false;

    if (x < 0)
    {
        x = -x;
        neg = true;
    }

    tmp = x & 0xffff;
    tmp *= 10000;
    tmp /= 0xffff;

    if (!neg)
        sprintf(str, "%d.%04u", x >> 16, tmp);
    else
        sprintf(str, "-%d.%04u", x >> 16, tmp);

    return str;
}
