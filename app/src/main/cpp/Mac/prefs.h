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
//        System Shock - ©1994-1995 Looking Glass Technologies, Inc.
//
//        Prefs.h    -    Handles saving and loading preferences.
//
//====================================================================================


//--------------------
//  Types
//--------------------
typedef struct
{
    int16_t        prefVer;                // Version - set to 0 for now.
    int16_t        prefPlayIntro;        // Play intro at startup if non-zero.

    // Game Options
    int16_t        goMsgLength;            // 0 - normal, 1 - brief
    bool    goPopupLabels;
    bool    goOnScreenHelp;
    int16_t        goLanguage;            // 0 - English

    // Sound Options
    bool    soBackMusic;
    bool    soSoundFX;
    int16_t        soMusicVolume;

    // Display Options
    int16_t        doResolution;            // 0 - High, 1 - Low
    int16_t        doDetail;                // 0 - Min, 1-Low, 2-High, 3-Max
    int16_t        doGamma;
    bool    doUseQD;

} ShockPrefs;

#define kPrefsFileType         'Sprf'
#define    kPrefsFileName        "\pSystem Shock Prefs"
#define kPrefsResID            'Pref'

//--------------------
//  Globals
//--------------------
extern ShockPrefs        gShockPrefs;

//--------------------
//  Prototypes
//--------------------
void SetDefaultPrefs(void);
OSErr LoadPrefs(ResType resID);
OSErr SavePrefs(ResType resID);
OSErr GetPrefsDir(int16_t *vRef, int32_t *parID);
