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
 * $Source: r:/prj/cit/src/RCS/musicai.c $
 * $Revision: 1.124 $
 * $Author: unknown $
 * $Date: 1994/11/26 03:16:18 $
 */

#define __MUSICAI_SRC

#include <string.h>
#include <stdlib.h>

#include "Shock.h"
#include "Prefs.h"

#include "musicai.h"
#include "MacTune.h"

#include "ai.h"
#include "faketime.h"
#include "map.h"
#include "mapflags.h"
#include "player.h"
#include "sfxlist.h"
#include "tools.h"
/*
#include <mainloop.h>
#include <_audio.h>
#include <_testing.h>
#include <objects.h>
#include <objcrit.h>
#include <texttool.h>
#include <otrip.h>
#include <diginode.h>
*/
#ifdef AUDIOLOGS
#include "audiolog.h"
#endif

//#include <ail.h>

extern void grind_music_ai();


uint8_t track_table[NUM_SCORES][SUPERCHUNKS_PER_SCORE];
uint8_t transition_table[NUM_TRANSITIONS];
uint8_t layering_table[NUM_LAYERS][MAX_KEYS];
uint8_t key_table[NUM_LAYERABLE_SUPERCHUNKS][KEY_BAR_RESOLUTION];

int8_t peril_bars = 0;

int32_t new_theme = 0;
int32_t new_x,new_y;
int32_t old_bore;
int16_t mai_override = 0;
uint8_t cyber_play = 255;

int32_t layer_danger = 0;
int32_t layer_success = 0;
int32_t layer_transition = 0;
int32_t transition_count = 0;
int8_t tmode_time = 0;
int32_t actual_score = 0;
uint8_t decon_count = 0;
uint8_t decon_time = 8;
bool in_deconst = false, old_deconst = false;
bool in_peril = false;
bool just_started = true;
int32_t score_playing = 0;
int16_t curr_ramp_time, curr_ramp;
int8_t curr_prioritize, curr_crossfade;

void musicai_clear();
errtype mai_transition(int32_t new_trans);

extern errtype make_request(int32_t chunk_num, int32_t piece_ID);
extern int32_t digifx_volume_shift(int16_t x, int16_t y, int16_t z, int16_t phi, int16_t theta, int16_t basevol);
extern int32_t digifx_pan_shift(int16_t x, int16_t y, int16_t z, int16_t phi, int16_t theta);
extern bool mai_semaphor;

uint8_t park_random = 75;
uint8_t park_playing = 0;
uint8_t access_random = 45;

uint32_t last_damage_sum = 0;
uint32_t last_vel_time =0;

// Damage taken decay & quantity of decay
int32_t danger_hp_level = 10;
int32_t danger_damage_level = 40;
int32_t damage_decay_time = 300;
int32_t damage_decay_amount = 6;
int32_t mai_damage_sum = 0;

// How long an attack keeps us in combat music mode
int32_t mai_combat_length = 1000;

bool bad_digifx = false;

//KLC - no longer need this    Datapath music_dpath;

#define SMALL_ROBOT_LAYER  3

int8_t mlimbs_machine = 0;

//------------------
//  INTERNAL PROTOTYPES
//------------------
errtype musicai_shutdown();
errtype musicai_reset(bool runai);
int32_t gen_monster(int32_t monster_num);


errtype musicai_shutdown()
{
    int32_t i;
    for (i=0; i < MLIMBS_MAX_SEQUENCES -1; i++)
        current_request[i].pieceID = 255;
    MacTuneKillCurrentTheme();
    return(OK);
}

extern bool run_asynch_music_ai;

errtype musicai_reset(bool runai)
{
    if (runai)                                                // Figure out if there is a theme to start with.
        grind_music_ai();
    mlimbs_counter = 0;
    return(OK);
}

void musicai_clear()
{
    mai_damage_sum = 0;
    last_damage_sum = 0;
    mlimbs_combat = 0;
}

void mlimbs_do_ai()
{
//    extern bool mlimbs_semaphore;
    extern errtype check_asynch_ai(bool new_score_ok);
    extern ObjID damage_sound_id;
    extern int8_t damage_sound_fx;

/* Is this really necessary?  It's already called twice in fr_rend().
#ifdef AUDIOLOGS
    audiolog_loop_callback();
#endif
*/
    // Play any queued sound effects, or damage SFX that have yet to get flushed
    if (damage_sound_fx != -1)
    {
        play_digi_fx_obj(damage_sound_fx,1,damage_sound_id);
        damage_sound_fx = -1;
    }

    if (music_on)
    {
        if (mlimbs_combat != 0)
        {
            if (mlimbs_combat < player_struct.game_time)
                mlimbs_combat = 0;
        }

        // Set danger layer
        layer_danger = 0;
        if (mai_damage_sum > danger_damage_level)
            layer_danger = 2;
        else if (player_struct.hit_points < danger_hp_level)
            layer_danger = 1;

        // Decay damage
        if ((last_damage_sum + damage_decay_time) < player_struct.game_time)
        {
            mai_damage_sum -= damage_decay_amount;
            if (mai_damage_sum < 0)
                mai_damage_sum = 0;
            last_damage_sum = player_struct.game_time;
        }

        if ((score_playing == BRIDGE_ZONE) && in_peril)
        {
            mlimbs_peril = DEFAULT_PERIL_MIN;
            mlimbs_combat  = 0;
        }
        else
        {
            if ((mlimbs_combat > 0) || in_peril)
            {
                mlimbs_peril = DEFAULT_PERIL_MAX;
            }
        }

        // KLC - moved here from grind_music_ai, so it can do this check at all times.
        if (global_fullmap->cyber)
        {
            MapElem     *pme;
            int32_t            play_me;

            pme = MAP_GET_XY(PLAYER_BIN_X, PLAYER_BIN_Y);                // Determine music for this
            if (!me_bits_peril(pme))                                                        // location in cyberspace.
                play_me = NUM_NODE_THEMES + me_bits_music(pme);
            else
                play_me = me_bits_music(pme);
            if (play_me != cyber_play)                                                        // If music needs to be changed, then
            {
                musicai_shutdown();                                                            // stop playing current tune
                make_request(0, play_me);                                                // setup new tune
                musicai_reset(false);                                                        // reset MLIMBS and
                MacTuneStartCurrentTheme();                                                // start playing the new tune.
            }
            else
                make_request(0, play_me);                                                // otherwise just queue up next tune.
            cyber_play = play_me;
        }

        // This is all pretty temporary right now, but here's what's happening.
        // If the gReadyToQueue flag is set, that means the 6-second timer has
        // fired.  So we call check_asynch_ai() to determine the next tune to play
        // then queue it up.
        //  Does not handle layering.  Just one music track!
        if (gReadyToQueue)
        {
            if (!global_fullmap->cyber)
                check_asynch_ai(true);
            int32_t    pid = current_request[0].pieceID;
            if (pid != 255)                                        // If there is a theme to play,
            {
                MacTuneQueueTune(pid);                        // Queue it up.
                mlimbs_counter++;                            // Tell mlimbs we've queued another tune.
                gReadyToQueue = false;
            }
        }

        // If a tune has finished playing, then another has just started, so prime the
        // timer to do the next tune calc.
        if (gTuneDone)
        {
            MacTunePrimeTimer();
            gTuneDone = false;
        }
    }
}


#ifdef NOT_YET  //

void mlimbs_do_credits_ai()
{
    extern bool mlimbs_semaphore;
    if (ai_cycle)
    {
        ai_cycle = 0;
        grind_credits_music_ai();
        mlimbs_preload_requested_timbres();
        mlimbs_semaphore = false;
    }
}

#endif //NOT_YET

errtype mai_attack()
{
    if (music_on)
    {
        mlimbs_combat = player_struct.game_time + mai_combat_length;
    }
    return(OK);
}

errtype mai_intro()
{
    if (music_on)
    {
        if (transition_table[TRANS_INTRO] != 255)
            mai_transition(TRANS_INTRO);
        mlimbs_peril = DEFAULT_PERIL_MIN;
        mlimbs_combat = 0;
    }
    return(OK);
}

errtype mai_monster_nearby(int32_t monster_type)
{
    if (music_on)
    {
        mlimbs_monster = monster_type;
        if (monster_type == NO_MONSTER)
        {
            mlimbs_combat = 0;
            mlimbs_peril = DEFAULT_PERIL_MIN;
        }
    }
    return(OK);
}

errtype mai_monster_defeated()
{
    if (music_on)
    {
        mlimbs_combat = 0;
    }
    return(OK);
}

errtype mai_player_death()
{
    if (music_on)
    {
        mai_transition(TRANS_DEATH);
        mlimbs_peril = DEFAULT_PERIL_MIN;
        peril_bars = 0;
        layer_danger = 0;
        mai_damage_sum = 0;
        layer_success = 0;
        mlimbs_machine = 0;
        mlimbs_monster = 0;
        mlimbs_combat = 0;
        musicai_shutdown();
        make_request(0, transition_table[TRANS_DEATH]);
        musicai_reset(false);
        MacTuneStartCurrentTheme();
    }
    return(OK);
}

errtype mlimbs_AI_init(void)
{
    mlimbs_boredom = 0;
    old_bore = 0;
    mlimbs_monster = NO_MONSTER;
    wait_flag = false;
    random_flag = 0;
    boring_count = 0;
    ai_cycle = 0;
    mlimbs_peril = DEFAULT_PERIL_MAX;
    current_transition = TRANS_INTRO;
    current_mode = TRANSITION_MODE;
    tmode_time = 1;    //KLC - was 4
    current_score = actual_score = last_score = WALKING_SCORE;
    current_zone = HOSPITAL_ZONE;
//    mlimbs_AI = &music_ai;
    cyber_play = 255;

    return(OK);
}

errtype mai_transition(int32_t new_trans)
{
    if ((next_mode == TRANSITION_MODE) || (current_mode == TRANSITION_MODE))
        return(ERR_NOEFFECT);

    if (transition_table[new_trans] < LAYER_BASE)
    {
        current_transition = new_trans;
        next_mode = TRANSITION_MODE;
        tmode_time = 1;    //KLC - was 4
    }
    else if ((transition_count == 0) && (layering_table[TRANSITION_LAYER_BASE + new_trans][0] != 255))
    {
        current_transition = new_trans;
//For now, let's not do any layered transitions.
//        transition_count = 1;        //KLC - was 2
    }
// temp
/*
int8_t    msg[30];
lg_sprintf(msg, "Transitioning:%d, mode:%d, count:%d", new_trans, next_mode, transition_count);
message_info(msg);
*/
    return(OK);
}

int32_t gen_monster(int32_t monster_num)
{
    if (monster_num < 3)
        return(0);
    if (monster_num < 6)
        return(1);
    return(2);
}

int32_t ext_rp = -1;

extern struct mlimbs_request_info default_request;

errtype make_request(int32_t chunk_num, int32_t piece_ID)
{
    current_request[chunk_num] = default_request;
    current_request[chunk_num].pieceID = piece_ID;

    // These get set all around differently and stuff
    current_request[chunk_num].crossfade = curr_crossfade;
    current_request[chunk_num].ramp_time = curr_ramp_time;
    current_request[chunk_num].ramp = curr_ramp;
    return(OK);
}

/*
errtype load_score_from_cfg(FSSpec *specPtr)
{
    int16_t      filenum;
    Handle    binHdl;
    Ptr        p;

    filenum = FSpOpenResFile(specPtr, fsRdPerm);
    if (filenum == -1)
        return (ERR_FOPEN);
    binHdl = GetResource('tbin', 128);
    if (binHdl == NULL)
        return (ERR_FOPEN);

    HLock(binHdl);
    p = *binHdl;
    BlockMoveData(p, track_table, NUM_SCORES * SUPERCHUNKS_PER_SCORE);
    p += NUM_SCORES * SUPERCHUNKS_PER_SCORE;
    BlockMoveData(p, transition_table, NUM_TRANSITIONS);
    p += NUM_TRANSITIONS;
    BlockMoveData(p, layering_table, NUM_LAYERS * MAX_KEYS);
    p += NUM_LAYERS * MAX_KEYS;
    BlockMoveData(p, key_table, NUM_LAYERABLE_SUPERCHUNKS * KEY_BAR_RESOLUTION);
    HUnlock(binHdl);

    CloseResFile(filenum);
    return(OK);
}
*/

int32_t old_score;

errtype fade_into_location(int32_t x, int32_t y)
    {
    MapElem *pme;

    new_x = x;
    new_y = y;
    new_theme = 2;
    pme = MAP_GET_XY(new_x,new_y);
    score_playing = me_bits_music(pme);

    // For going into/outof elevator and cyberspace, don't do any crossfading.
    if ((score_playing == ELEVATOR_ZONE) || (score_playing > CYBERSPACE_SCORE_BASE) ||
         (old_score == ELEVATOR_ZONE) || (old_score > CYBERSPACE_SCORE_BASE))
    {
        if (old_score != score_playing)                        // Don't restart music if going from elevator
        {                                                                    // to elevator (eg, when changing levels).
            load_score_for_location(new_x,new_y);
            MacTuneStartCurrentTheme();
            new_theme = 0;
        }
    }
    else    // for now, we're not going to do any cross-fading.  Just load the new score.
    {
//        message_info("Sould be fading into new location.");
        load_score_for_location(new_x,new_y);
        MacTuneStartCurrentTheme();
        new_theme = 0;
    }
    return(OK);
}

/*KLC - don't need
errtype blank_theme_data()
{
    memset(track_table, 255, NUM_SCORES * SUPERCHUNKS_PER_SCORE * sizeof(uint8_t));
    memset(transition_table, 255, NUM_TRANSITIONS * sizeof(uint8_t));
    memset(layering_table, 255, NUM_LAYERS * MAX_KEYS * sizeof(uint8_t));
    memset(key_table, 255, NUM_LAYERABLE_SUPERCHUNKS * KEY_BAR_RESOLUTION * sizeof(uint8_t));
    return(OK);
}
*/

// don't need?      bool voices_4op = false;
// don't need?      bool digi_gain = false;
void load_score_guts(int8_t score_playing)
{
    int32_t         rv;
    int8_t         base[20], temp[30];
    FSSpec    themeSpec;

    strcpy(base, "Theme");                            // Get the theme file name.
    numtostring(score_playing, temp);
    strcat(base, temp);

// temp
/*
strcpy(temp, "Loading: ");
strcat(temp, base);
message_info(temp);
*/
    FSMakeFSSpec(gDataVref, gDataDirID, c2pstr(base), &themeSpec);

    musicai_shutdown();
    rv = MacTuneLoadTheme(&themeSpec, score_playing);
    if (rv == 1)
        musicai_reset(true);
    else  // handle this a better way.
        DebugStr("\pLoad theme failed!");
}

errtype load_score_for_location(int32_t x, int32_t y)
{
    MapElem *pme;
    int8_t sc;
    extern int8_t old_bits;

    pme = MAP_GET_XY(x,y);
    sc = me_bits_music(pme);
//KLC    if ((global_fullmap->cyber) && (sc != 0))
    if (global_fullmap->cyber)
        sc = CYBERSPACE_SCORE_BASE;
    old_bits = old_score = score_playing = sc;
    if (sc == 7)                                                    // Randomize boredom for the elevator
        mlimbs_boredom = TickCount() % 8;
    else
        mlimbs_boredom = 0;
    load_score_guts(sc);
    return(OK);
}

#ifdef NOT_YET  //

// 16384
// 8192
//#define SFX_BUFFER_SIZE     8192
#define MIDI_TYPE    0
#define DIGI_TYPE    1
// #define SPCH_TYPE    2     // perhaps someday, for special CD speech and separate SB digital effects, eh?
#define DEV_TYPES    2

#define DEV_CARD  0
#define DEV_IRQ    1
#define DEV_DMA    2
#define DEV_IO     3
#define DEV_DRQ    4
#define DEV_PARMS 5

// doug gets sneaky, film at 11
#define MIDI_CARD MIDI_TYPE][DEV_CARD
#define MIDI_IRQ  MIDI_TYPE][DEV_IRQ
#define MIDI_DMA  MIDI_TYPE][DEV_DMA
#define MIDI_IO    MIDI_TYPE][DEV_IO
#define MIDI_DRQ  MIDI_TYPE][DEV_DRQ
#define DIGI_CARD DIGI_TYPE][DEV_CARD
#define DIGI_IRQ  DIGI_TYPE][DEV_IRQ
#define DIGI_DMA  DIGI_TYPE][DEV_DMA
#define DIGI_IO    DIGI_TYPE][DEV_IO
#define DIGI_DRQ  DIGI_TYPE][DEV_DRQ

#define SFX_BUFFER_SIZE 8192
//#define SFX_BUFFER_SIZE 4096

static int8_t *dev_suffix[]={"card","irq","dma","io","drq"};
static int8_t *dev_prefix[]={"midi_","digi_"};

int16_t music_get_config(int8_t *pre, int8_t *suf)
{
    int32_t tmp_in, dummy_count=1;
    int8_t buf[20];
    strcpy(buf,pre);
    strcat(buf,suf);
    if (!config_get_value(buf, CONFIG_INT_TYPE, &tmp_in, &dummy_count))
        return -1;
    else
        return (int16_t)tmp_in;
}

audio_card *fill_audio_card(audio_card *cinf, int16_t *dinf)
{
    cinf->type=dinf[DEV_CARD];        cinf->dname=NULL;
    cinf->io=dinf[DEV_IO];             cinf->irq=dinf[DEV_IRQ];
    cinf->dma_8bit=dinf[DEV_DMA];    cinf->dma_16bit=-1;        // who knows, eh?
    return cinf;
}

#ifdef PLAYTEST
static int8_t def_sound_path[]="r:\\prj\\cit\\src\\sound";
#else
static int8_t def_sound_path[]="sound";
#endif

#ifdef SECRET_SUPPORT
FILE *secret_fp=NULL;
int8_t secret_dc_buf[10000];
volatile int8_t secret_update=false;
void secret_closedown(void)
{
    if (secret_fp!=NULL)
        fclose(secret_fp);
}
#endif

#endif //NOT_YET

//----------------------------------------------------------------------
//  For Mac version, the vast majority of the config mess just goes away.  But we do check for
//  the presence of QuickTime Musical Instruments.
//----------------------------------------------------------------------
errtype music_init()
{
/* put in later
    int32_t i,j;
    bool gm=false;
    int16_t dev_info[DEV_TYPES][DEV_PARMS];
    int8_t s[64],path[64];
    audio_card card_info;
    extern uint8_t curr_sfx_vol;
    extern int8_t curr_vol_lev;

#ifdef SECRET_SUPPORT
    if ((secret_fp=fopen("secret.ddb","wt"))!=NULL)
    {
        secret_dc_buf[0]='\0';
        mono_clear();
        mono_split(MONO_AXIS_Y,4);
        mono_setwin(2);
    }
    atexit(secret_closedown);
#endif

    strcpy(s,def_sound_path);

    dev_info[MIDI_CARD]=music_get_config(dev_prefix[0],dev_suffix[0]);
    dev_info[DIGI_CARD]=music_get_config(dev_prefix[1],dev_suffix[0]);

    DatapathClear(&music_dpath);

    // can we make this actually know what is going on?
    switch (dev_info[MIDI_CARD])
    {      // probably should be in the library, not here...
    case GRAVISULTRASTUPID: case MT32: case GENMIDI: case AWE32: case SOUNDSCAPE: case RAP_10: gm=true; break;
    }

    // add contents of CFG_SOUNDVAR
    if (config_get_raw(CFG_SOUNDVAR,path,64))
    {
//        mprintf("hey, path = %s\n",path);
        DatapathAdd(&music_dpath, path);
        if (gm)
            { strcat(path,"\\genmidi"); }
        else
            { strcat(path,"\\sblaster"); }
//        mprintf("now, path = %s\n",path);
        DatapathAdd(&music_dpath,path);
    }

    // add contents of CFG_CD_SOUNDVAR
    if (config_get_raw(CFG_CD_SOUNDVAR,path,64))
    {
//        mprintf("hey, path = %s\n",path);
        DatapathAdd(&music_dpath, path);
        if (gm)
            { strcat(path,"\\genmidi"); }
        else
            { strcat(path,"\\sblaster"); }
//        mprintf("now, path = %s\n",path);
        DatapathAdd(&music_dpath,path);
    }

#ifdef PLAYTEST
    DatapathAdd(&music_dpath,s+15); // and go back and add net/sound if necessary
#else
    DatapathAdd(&music_dpath,s);
#endif

    if (gm)
        { strcat(s,"\\genmidi"); }
    else
        { strcat(s,"\\sblaster"); }

#ifdef PLAYTEST
    DatapathAdd(&music_dpath,s+15);          // add just sound/devtype
    DatapathAdd(&music_dpath,s);
    s[21] = '\0';
    DatapathAdd(&music_dpath,s);
#else
    DatapathAdd(&music_dpath,s);          // add just sound/devtype
#endif

    snd_setup(&music_dpath,"sound/cit");        // really it should go find cit.ad on the datapath, then use its path

    music_card=(dev_info[MIDI_CARD]>0);
    sfx_card  =(dev_info[DIGI_CARD]>0);
    if (!(music_card||sfx_card))
    {
        curr_sfx_vol = 0;
        curr_vol_lev = 0;
        return(ERR_NODEV);
    }

    for (i=0; i<DEV_TYPES; i++)
        if (dev_info[i][DEV_CARD]!=-1)
            for (j=1; j<DEV_PARMS; j++)
                dev_info[i][j]=music_get_config(dev_prefix[i],dev_suffix[j]);

#define ULTRA_GROSS_JOHN_MILES_HACK
#ifdef ULTRA_GROSS_JOHN_MILES_HACK
    {
        #include <conio.h>
        if ((dev_info[MIDI_CARD]==GENMIDI)&&(dev_info[DIGI_CARD]==SOUNDBLASTERPRO2)) {
            int32_t mod_loc=dev_info[DIGI_IO];        // loc, the io port to send too
            if (mod_loc==-1) mod_loc=0x220;      // i know much secretness of destruction
            outp(mod_loc+4,0x83);      // such that def io is 220, which AIL wont
            outp(mod_loc+5,0xb);        // tell me till later, when we init it
        }                                                  // which we are not allowed to do yet
    }
#endif

    if (music_card)
    {
        if (snd_start_midi(fill_audio_card(&card_info,dev_info[MIDI_TYPE]))!=SND_OK)
        {
            Warning("Device %d not loaded for Midi at %x %x %x %x\n",dev_info[MIDI_CARD],dev_info[MIDI_IO],dev_info[MIDI_IRQ],dev_info[MIDI_DMA],dev_info[MIDI_DRQ]);
            music_card = false;
            curr_vol_lev = 0;
        }
        else
        {
            mlimbs_init();
        }
    }
    else
        curr_vol_lev = 0;

    if (sfx_card)
    {
        if (snd_start_digital(fill_audio_card(&card_info,dev_info[DIGI_TYPE]))!=SND_OK)
        {
            Warning("Device %d not loaded for DigiFx at %x %x %x %x\n",dev_info[DIGI_CARD],dev_info[DIGI_IO],dev_info[DIGI_IRQ],dev_info[DIGI_DMA],dev_info[DIGI_DRQ]);
            sfx_card = false;
            curr_sfx_vol = 0;
        }
        else  // note this use to allocate double buffer space here
        {
            snd_set_digital_channels(cur_digi_channels);
//            digi_gain = true;                 // ie look at detail and stuff
        }
    }
    else
        curr_sfx_vol = 0;

    if (sfx_card)
    {
        sfx_on=true;
#ifdef AUDIOLOGS
        audiolog_init();
#endif
    }
*/
        if (gShockPrefs.soBackMusic)
        {
//    if (music_card)
//    {
                    if (MacTuneInit() == 0)                        // If no error, go ahead and start up.
                    {
                music_on = mlimbs_on = true;
                mlimbs_AI_init();
            }
            else                                                    // else turn off the music globals and prefs
            {
                gShockPrefs.soBackMusic = false;
                SavePrefs(kPrefsResID);
                music_on = mlimbs_on = false;
            }
//    }
        }
        else
        {
            music_on = mlimbs_on = false;
        }
    return(OK);
}

/* KLC - doesn't do anything
void music_free(void)
{
    DatapathFree(&music_dpath);
}
*/
