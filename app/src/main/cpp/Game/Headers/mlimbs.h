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
#ifndef __MLIMBS_H
#define __MLIMBS_H
/*
 * $Source: r:/prj/cit/src/inc/RCS/mlimbs.h $
 * $Revision: 1.14 $
 * $Author: dc $
 * $Date: 1994/11/19 20:44:53 $
 */

// really this is in sndcall.c, but heck
void synchronous_update(void);
void sound_frame_update(void);

//KLC what's the diff between a char and a signed char?      typedef signed char schar;

/* defines */
#define MAX_SEQUENCES                        80
#define CALLBACK_SEQ_NUM                0
#define HUGE_PRIORITY                             10000
#define MLIMBS_MAX_CHANNELS             8
#define MLIMBS_MAX_SEQUENCES        8
#define MLIMBS_TIMER_FREQUENCY     120

#define MLIMBS_STOPPED                    0
#define MLIMBS_PLAYING_PIECE            1

#define SEQUENCE_CHANNEL_PENDING        -1
#define SEQUENCE_CHANNEL_MUTED        -2
#define SEQUENCE_CHANNEL_UNUSED        -3

#define DEFAULT_REL_VOL                    100
#define DEFAULT_RAMP_TIME                0

// referent to mlimbs generated sequences
#define MLIMBS_REF            0xFF0000
#define mrefBuild(themeid,seq) (MLIMBS_REF|(themeid<<16)|seq)

#define CALLBACK_ON

/* XMIDI_info contains 'permanent' information about a given piece. */
struct mlimbs_piece_info
{
    uint8_t max_voices;                // Maximum number of voices this sequence uses at once.
    uint8_t avg_voices;
    uint16_t channel_map;            // Bit map of what channels this sequence uses.
    uint8_t num_measures;
    int32_t priority;                        // Base priority of this sequence.
    uint8_t channel_voices[7];    // channel_voice[i] = # of voices channel i+11 uses in this sequence.
};

struct mlimbs_request_info
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

struct mlimbs_channel_info
{
    int32_t usernum;                        // Index into userID[] for the chunk currently using this channel
    int32_t sequence_channel;            //    For channels used by sequences, this field shows which of the
                                            // sequences channels should be mapped to this channel.
    int32_t mchannel;                          // this channels existance
    int8_t status;                        // status can be one of the following
};

struct mlimbs_playing_info
{
    int32_t     pieceID;
    uint16_t current_channel_map;
    int16_t  sequence_channel_status[7];    // Status of the sequence channel, >= 0 is mlimbs physical channel
    int8_t    seq_id;
    uint32_t    rel_vol;
    bool    channel_prioritize;
    int8_t     crossfade_status;                    // <= 0 - no fade, >= 10 & <= 16, next channel to fade in or fade out.
};

extern volatile struct mlimbs_request_info current_request[MLIMBS_MAX_SEQUENCES - 1];

extern int8_t                         mlimbs_status;    // could make this one bitfield of status, on/off, enable/not, so on
extern bool                         mlimbs_on;
extern volatile int32_t         mlimbs_error;
extern volatile uint32_t         default_rel_vol;
extern volatile uint32_t         default_ramp_time;
extern volatile uint8_t    num_XMIDI_sequences;
extern volatile uint32_t    mlimbs_counter;
extern volatile void  (*mlimbs_AI)();
extern volatile int32_t        mlimbs_master_slot;

/* Function prototypes */
int32_t    mlimbs_init(void);
//void cdecl  mlimbs_callback(snd_midi_parms *mprm, int32_t trigger_value);
//void cdecl  mlimbs_seq_done_call(snd_midi_parms *mprm);

void    mlimbs_shutdown(void);
int32_t    mlimbs_load_theme(int8_t *, int8_t *, int);
void    mlimbs_stop_theme(void);
int32_t    mlimbs_start_theme(void);
void    mlimbs_purge_theme(void);
void    mlimbs_mute_sequence_channel(int32_t usernum, int32_t x,bool);
int32_t    mlimbs_unmute_sequence_channel(int32_t usernum, int32_t x);
int32_t    mlimbs_channel_prioritize(int32_t priority, int32_t pieceID, int32_t voices_needed,bool crossfade,bool channel_prioritize);
int32_t    mlimbs_assign_channels(int32_t,bool);
int32_t    mlimbs_play_piece(int32_t, int32_t, int32_t, int32_t, bool,bool);
int32_t    mlimbs_punt_piece(int32_t);
int8_t    mlimbs_get_crossfade_status(int32_t);
void    mlimbs_reassign_channels(void);
void    mlimbs_timer_callback(void);
void    mlimbs_change_master_volume(int32_t);
void    mlimbs_change_relative_volume(int32_t, int32_t, int);
void    mlimbs_change_relative_tempo(int32_t, int32_t, int);
void    mlimbs_return_to_synch(void);
void    mlimbs_preload_full_timbres_and_go_asynch(void);
void    mlimbs_preload_requested_timbres(void);
#endif // __MLIMBS_H
