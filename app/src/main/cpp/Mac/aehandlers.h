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
//====================================================================
//
//        System Shock - Macintosh.
//
//        High-Level Event handling code.
//
//====================================================================

#include <AppleEvents.h>

void                DoAEInstallation(void);
OSErr             RequiredCheck(AppleEvent *theAppleEvent);
pascal OSErr    HandleOAPP(AppleEvent *theAppleEvent, AppleEvent *reply, int32_t myRefCon);
pascal OSErr    HandleODOC(AppleEvent *theAppleEvent, AppleEvent *reply, int32_t myRefCon);
pascal OSErr    HandlePDOC(AppleEvent *theAppleEvent, AppleEvent *reply, int32_t myRefCon);
pascal OSErr    HandleQUIT(AppleEvent *theAppleEvent, AppleEvent *reply, int32_t myRefCon);
