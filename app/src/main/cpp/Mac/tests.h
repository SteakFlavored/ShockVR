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
//===========================================================
//
//		System Shock - ©1994-1995 Looking Glass Technologies, Inc.
//
//		Tests.h	-	Code to do test commands.
//
//===========================================================


//--------------------
//  Constants
//--------------------

//--------------------
//  Globals
//--------------------
extern int16_t		gCurrTest;

//--------------------
//  Prototypes
//--------------------
void SetupTests(void);
void DoTestClick(Point localPt);
void DoTestUpdate(WindowPtr wnd);

void DoTestBrowseImages(void);
void DoTestBrowseFonts(void);
void DoTestLoadPalette(void);
void DoTestMoveKeys(void);
void DoTestMouse(void);
void DoPlayMovie(int16_t cmd);
void DoPlayCutScene(int16_t cmd);
void DoPlayAudioLog(int16_t cmd);
void DoLoadLevelMap(int16_t cmd);
void DoZoomCurrMap(int16_t cmd);
void RenderTest(void);
