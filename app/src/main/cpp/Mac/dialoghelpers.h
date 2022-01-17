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
//====================================================================================
//
//		System Shock - ©1994-1995 Looking Glass Technologies, Inc.
//
//		DialogHelpers.h	-	Utility routines and user items for dialog boxes.
//
//====================================================================================


//--------------------
//  Constants
//--------------------
#define kBtnOK				1
#define kBtnCancel			2
#define	kUsrOKOutline	3

#define kSliderWidth		112
#define kSliderHeight		12
#define kSliderEnd			6
#define	kSliderBackPict			8990
#define	kThumbIcon				8991
#define	kSliderBackPictDim	8992
#define	kThumbIconDim			8993

typedef pascal void (*SliderCallbackProcPtr)(int16_t value);

//--------------------
//  Globals
//--------------------
extern bool		gGrayOK;
extern bool		gIgnoreGray;
extern Str255		gDimTitle;
extern bool		gDimmed;
extern MenuHandle	gPopupMenuHdl;
extern int16_t			gPopupSel;
extern int16_t			gSliderLastPos;
extern bool		gSliderDimmed;

extern int16_t			gNewGameSel[4];

//--------------------
//  Prototypes
//--------------------
void FlashButton(WindowPtr dlog, int16_t itemN);
pascal void OKButtonUser(WindowPtr dlog, int16_t itemN);

pascal bool ShockFilterProc(DialogPtr dlog, EventRecord *evt, int16_t *itemHit);
pascal bool ShockAlertFilterProc(DialogPtr dlog, EventRecord *evt, int16_t *itemHit);

pascal void DimStaticUser(WindowPtr dlog, int16_t itemN);

pascal void PopupMenuUser(WindowPtr dlog, int16_t itemN);
pascal void PopupMenuUserNG(WindowPtr dlog, int16_t itemN);

pascal void GroupBoxUser(WindowPtr dlog, int16_t itemNum);

void SetupSlider(void);
void SetSliderBitmaps(void);
pascal void DrawSlider(WindowPtr dlog, int16_t itemNum);
int16_t DoSliderTracking(WindowPtr dlog, int16_t itemNum, SliderCallbackProcPtr cb);
void FreeSlider(void);
