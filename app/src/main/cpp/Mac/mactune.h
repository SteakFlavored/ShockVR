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
//==============================================================================
//
//        System Shock - ©1994-1995 Looking Glass Technologies, Inc.
//
//        MacTune.h    -    Rewrite of Shock's MLIMBS.H file to use QuickTime MIDI rather than AIL.
//
//==============================================================================


#include <Timer.h>

//-----------------
//  TYPES & DEFINES
//-----------------
typedef struct
{
    TMTask            task;                        // The actual TimeManager task structure
    int32_t                appA5;                    // We need this silly thing for 68K programs
}
CalcTuneTask, *CalcTuneTaskPtr;

typedef struct mlimbs_request_info
{
    int32_t pieceID;                        //  Indexes an array of XMIDI_info structs.  Specifies which piece to play.
    int32_t priority;                        // Priority of this request.
    int32_t loops;                            // Number of loops. -1 => until deliberately stopped.
    uint32_t rel_vol;                        // Specifies at what relative volume to play it at. (percent)
    uint32_t ramp_time;                    // Specifies the time to ramp in to the specified rel_vol, or ramp out to 0.
    int32_t pan;                                // Note that this pan value affects all channels.
    bool channel_prioritize;
    int8_t    crossfade;                    // 0 - don't crossfade,  <0 - crossfade out, >0 - crossfade in.
    int8_t ramp;                            // 0 - don't ramp,         <0 - ramp out         >0 - ramp in
     uint8_t pad;
};

#define MLIMBS_MAX_SEQUENCES        8
#define MLIMBS_MAX_CHANNELS             8


//-----------------
//  EXTERN GLOBALS
//-----------------
extern bool        mlimbs_on;
extern int8_t    mlimbs_status;

extern mlimbs_request_info    current_request[MLIMBS_MAX_SEQUENCES - 1]; // Request information

extern uint32_t    mlimbs_counter;
extern int32_t        mlimbs_error;
extern bool        mlimbs_semaphore;

extern Handle            gHeaderHdl, gTuneHdl, gOfsHdl;        // Holds the tune-related data for the current theme file.
extern int32_t                *gOffsets;                                    // Array of offsets for the beginning of each tune.
extern TunePlayer    gPlayer;                                    // The Tune Player.
extern bool        gTuneDone;                                    // True when a sequence has finished playing (set by CB proc).
extern bool        gReadyToQueue;                            // True when it's time to queue up a new sequence.

extern TuneCallBackUPP    gTuneCBProc;                        // The tune's callback proc.
extern CalcTuneTask        gCalcTuneTask;                    // Global to hold task info.
extern TimerUPP            gCalcTuneProcPtr;                // UPP for the 6-second time manager tune determiner task.


//-----------------
//  PROTOTYPES
//-----------------
int32_t    MacTuneInit(void);
void    MacTuneShutdown(void);
int32_t    MacTuneLoadTheme(FSSpec *themeSpec, int32_t themeID);
void    MacTuneStartCurrentTheme(void);
void    MacTuneKillCurrentTheme(void);
void    MacTunePurgeCurrentTheme(void);
void     MacTunePlayTune(int32_t tune);
void     MacTuneQueueTune(int32_t tune);
void    MacTunePrimeTimer(void);
