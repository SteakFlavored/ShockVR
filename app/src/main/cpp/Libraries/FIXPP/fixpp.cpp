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
 * $Header: r:/prj/lib/src/fixpp/RCS/fixpp.cc 1.28 1994/08/30 11:32:42 jak Exp $
 */

#include <stdio.h>

#include "fixpp.h"

// Moved here from header file - KC
Fixpoint rawConstruct( int32_t l )
{
	Fixpoint f;
	f.val = l;
	return f;
}
#define f2Fixpoint(x) (rawConstruct( (int32_t)((x)*SHIFTMULTIPLIER) ))

Fixpoint Fixpoint_one_over_two_pi = f2Fixpoint(0.159154943);
Fixpoint Fixpoint_two_pi          = f2Fixpoint(6.283185306);
