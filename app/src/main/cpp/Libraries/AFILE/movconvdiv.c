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
//    ==============================================================
//        Convert a raw QT movie (from the LG .MOV file) into a nice compressed movie.
//        Make a separate movie for each palette change.
//    ==============================================================

#include <stdlib.h>
#include <string.h>

#include "lg.h"
#include "rect.h"
#include "fix.h"
#include "quiktime.h"
#include "2d.h"

#include "InitMac.h"
#include "ShockBitmap.h"
extern Ptr    gScreenAddress;
extern int32_t    gScreenRowbytes;

#include <Movies.h>
#include <ImageCompression.h>
#include <QuickTimeComponents.h>
#include <Sound.h>

typedef struct
{
    int32_t     frameNum;
    int16_t    palID;
} PalChange;

//--------------------------
//  Globals the user should set
//--------------------------
int8_t        gInputMov[] = "INTRO.QTM";
FSSpec        gInputPal = { 0, 0, "\pIntro Palettes" };
FSSpec        gInputSnd = { 0, 0, "\pINTRO" };

#define codec        'smc '
#define spatialQ        codecHighQuality
#define temporalQ    codecHighQuality
#define codecType    bestCompressionCodec
#define kPrevious    codecFlagUpdatePreviousComp

//  Globals
WindowPtr        gMainWindow;
extern int16_t        gMainVRef;
int16_t            gResNum;
uint32_t            *gSampleTimes;
uint32_t            *gChunkOffsets;
int32_t                gNumFrames;

ImageDescription    **gImageDescriptionH = 0L;        // Contains info about the sample
Movie            gMovie = 0;                            // Our movie, track and media
Track            gTrack;
Media            gMedia;
int16_t            gMovieResNum;
Rect            gMovieRect;
Handle            gCompressedFrameBitsH = 0L;            // Buffer for the compressed data
ImageSequence    gSeq;

QT_ChunkInfo chunkInfo[] =
{
    QT_CLIP,false,
    QT_CRGN,true,
    QT_DINF,false,
    QT_DREF,true,
    QT_EDTS,false,
    QT_ELST,true,
    QT_HDLR,true,
    QT_KMAT,true,
    QT_MATT,false,
    QT_MDAT,true,
    QT_MDIA,false,
    QT_MDHD,true,
    QT_MINF,false,
    QT_MOOV,false,
    QT_MVHD,true,
    QT_SMHD,true,
    QT_STBL,false,
    QT_STCO,true,
    QT_STSC,true,
    QT_STSD,true,
    QT_STSH,true,
    QT_STSS,true,
    QT_STSZ,true,
    QT_STTS,true,
    QT_TKHD,true,
    QT_TRAK,false,
    QT_UDTA,false,
    QT_VMHD,true,
    0,0
};
TrackType currTrackType;

//  Prototypes
void main(void);
void     CheckError(OSErr error, Str255 displayString);
void SetInputSpecs(void);
void SetPalette(int16_t palID);
void CreateAMovie(void);
void EndAMovie(void);

bool QuikReadChunkHdr(FILE *fp, QT_ChunkHdr *phdr);
QT_ChunkInfo *QuikFindChunkInfo(QT_ChunkHdr *phdr);
void QuikSkipChunk(FILE *fp, QT_ChunkHdr *phdr);

void CreateMySoundTrack(Movie theMovie);
void CreateSoundDescription(Handle sndHandle, SoundDescriptionHandle    sndDesc,
                            int32_t *sndDataOffset, int32_t *numSamples, int32_t *sndDataSize);
int32_t GetSndHdrOffset(Handle sndHandle);

void MyCreateTextTrack(Movie theMovie);


//    ---------------------------------------------------------------
//        MAIN PROGRAM
//    ---------------------------------------------------------------

void main(void)
{
    grs_screen         *screen;
    Ptr                p;
    int32_t                stupid;
    OSErr            result;

    FILE             *fp;
    uint8_t             *dbuff;
    uint32_t             dbuffLen;
    QT_ChunkHdr     chunkHdr;
    QT_ChunkInfo     *pinfo;

    int16_t            palResNum, sndResNum;
    Handle            palChgHdl;
    PalChange        *pcp;

    Rect            r;
    int32_t                compressedFrameSize;                    /* Size of current compressed frame */

    //---------------------
    //    Init graphics system
    //---------------------
    InitMac();
    CheckConfig();

    SetupWindows(&gMainWindow);                                // setup everything
    SetupOffscreenBitmaps();

    gr_init();
    gr_set_mode (GRM_640x480x8, true);
    screen = gr_alloc_screen (grd_cap->w, grd_cap->h);
    gr_set_screen (screen);
     SetRect(&gMovieRect,0,0,600,300);

    //---------------------
    //    Setup the input FileSpecs
    //---------------------
    SetInputSpecs();

    //---------------------
    // Setup Quicktime stuff.
    //---------------------
    if (EnterMovies() != noErr)    // Start up the movie tools
    {
        ParamText("\pCan't startup QuickTime.", "\p", "\p", "\p");
        StopAlert(1000, nil);
        CleanupAndExit();
    }

    //----------------------
    //    Open the input QuickTime movie.
    //----------------------
    fp = fopen(gInputMov, "rb");
    if (fp == NULL)
        CheckError(1, "\pCan't open the input movie!!");

    dbuffLen = 64000;
    dbuff = (uint8_t *)malloc(dbuffLen);
/*
    //----------------------
    //    Open the input Sound file.
    //----------------------
    sndResNum = FSpOpenResFile(&gInputSnd, fsRdPerm);
    if (sndResNum == -1)
        CheckError(1, "\pCan't open the sound file!!");
*/
    //----------------------
    //    Open the input Palettes file, read the palette changes, and set the first palette.
    //----------------------
    palResNum = FSpOpenResFile(&gInputPal, fsRdPerm);
    if (palResNum == -1)
        CheckError(1, "\pCan't open the palette file!!");
    palChgHdl = GetResource('pchg', 128);
    if (!palChgHdl)
        CheckError(1, "\pCan't load the palette changes resource!!");
    SetPalette(128);

    // Allocate a description handle for the movies.
    gImageDescriptionH = (ImageDescription **)NewHandle(sizeof(ImageDescription));
    CheckError(MemError(), "\pCan't alloc description for video.");

    // Create the first movie.
    CreateAMovie();

    //-----------------------------------
    //    Get the Chunk Offset and Sample-to-Time tables.
    //-----------------------------------
    while (true)
    {
        if (!QuikReadChunkHdr(fp, &chunkHdr))
            break;
        if (chunkHdr.length == 0)
            break;
        pinfo = QuikFindChunkInfo(&chunkHdr);
        if (pinfo->isleaf)
        {
            if ((chunkHdr.ctype != QT_MDAT))
            {
                if (chunkHdr.length > dbuffLen)
                {
                    dbuffLen = chunkHdr.length;
                    dbuff = (uint8_t *)realloc(dbuff, dbuffLen);
                }
                fread(dbuff, chunkHdr.length - sizeof(QT_ChunkHdr), 1, fp);

                // For the sample-to-time table, create our own table giving a time
                // for each frame.
                if (chunkHdr.ctype == QT_STTS)
                {
                    QTS_STTS    *p = (QTS_STTS *)dbuff;
                    int16_t        i, j, si;

                    gSampleTimes = (uint32_t *)malloc(1200 * sizeof(uint32_t));
                    si = 0;
                    for (i = 0; i < p->numEntries; i++)
                        for (j = 0; j < p->time2samp[i].count; j++)
                            gSampleTimes[si++] = p->time2samp[i].duration;
                }

                // For the chunk offsets table, read it in to a memory block.
                if (chunkHdr.ctype == QT_STCO)
                {
                    QTS_STCO     *p = (QTS_STCO *)dbuff;

                    gNumFrames = p->numEntries;
                    gChunkOffsets = (uint32_t *)malloc(p->numEntries * sizeof(uint32_t));
                    BlockMove(p->offset, gChunkOffsets, p->numEntries * sizeof(uint32_t));
                }
            }
            else
                QuikSkipChunk(fp, &chunkHdr);
        }
    }

    //----------------------------
    //    Show the movie, one frame at a time.
    //----------------------------
    {
        int32_t            f, line;
        Ptr            frameBuff;
        Ptr            imgp, scrp;
        RGBColor    black = {0, 0, 0};
        RGBColor    white = {0xffff, 0xffff, 0xffff};
        int8_t        buff[64];
        PalChange    *pc;

        frameBuff = malloc(600 * 300);
        CheckError(MemError(), "\pCan't allocate a frame buffer for input movie.");

        SetRect(&r, 0, -17, 300, 0);
        for (f = 0; f < gNumFrames; f++)
        {
            fseek(fp, gChunkOffsets[f], SEEK_SET);
            fread(frameBuff, 600 * 300, 1, fp);

            // Check for a palette change.  If one occurs, then finish up the current movie,
            // change the palette, then open a new movie.
            HLock(palChgHdl);
            pc = (PalChange *)*palChgHdl;
            while (pc->frameNum)
            {
                if (pc->frameNum == f)
                {
                    EndAMovie();
                    SetPalette(pc->palID);
                    CreateAMovie();
                    break;
                }
                pc++;
            }
            HUnlock(palChgHdl);

            imgp = frameBuff;
            scrp = gScreenAddress;
            for (line = 0; line < 300; line++)
            {
                BlockMove(imgp, scrp, 600);
                imgp += 600;
                scrp += gScreenRowbytes;
            }

            // Display the frame number.

            sprintf(buff, "Frame: %d", f);
            RGBForeColor(&black);
            PaintRect (&r);
            MoveTo(1, -6);
            RGBForeColor(&white);
            DrawText(buff, 0, strlen(buff));

            // Add the frame to the QuickTime movie.

            HLock(gCompressedFrameBitsH);
            result = CompressSequenceFrame(gSeq,
                ((CGrafPort *)(gMainWindow))->portPixMap,
                &gMovieRect,
                kPrevious, *gCompressedFrameBitsH, &compressedFrameSize, 0L, 0L);
            CheckError(result, "\pCan't compress a frame.");
            HUnlock(gCompressedFrameBitsH);

            result = AddMediaSample(gMedia, gCompressedFrameBitsH, 0L, compressedFrameSize,
                                    gSampleTimes[f], (SampleDescriptionHandle)gImageDescriptionH,1L,0, 0L);
            CheckError(result, "\pCan't add the frame sample.");
        }
    }

    EndAMovie();

    if (gImageDescriptionH)
        DisposeHandle((Handle)gImageDescriptionH);

    CloseResFile(palResNum);
    CloseResFile(sndResNum);
    fclose(fp);
     ExitMovies();

    gr_clear(0xFF);
    CleanupAndExit();
}

//------------------------------------------------------------------------
//  Exit in case of an error.
//------------------------------------------------------------------------
void     CheckError(OSErr error, Str255 displayString)
{
    if (error == noErr)
        return;
    ParamText(displayString, "\p", "\p", "\p");
    StopAlert(1000, nil);
     ExitMovies();
    CleanupAndExit();
}

//------------------------------------------------------------------------
//  Setup the input file specs.
//------------------------------------------------------------------------
void SetInputSpecs(void)
{
    int16_t    vRefNum;
    int32_t        temp, parID;

     GetWDInfo(gMainVRef, &vRefNum, &parID, &temp);

    gInputPal.vRefNum = vRefNum;
    gInputPal.parID = parID;

    gInputSnd.vRefNum = vRefNum;
    gInputSnd.parID = parID;
}

//------------------------------------------------------------------------
//  Load in the 'mpal' resource and set it.
//------------------------------------------------------------------------
void SetPalette(int16_t palID)
{
    Handle    gPalHdl;

    gPalHdl = GetResource('mpal', palID);
    if (!gPalHdl)
    {
        ParamText("\pCan't load a palette resource!!", "\p", "\p", "\p");
        StopAlert(1000, nil);
         ExitMovies();
        CleanupAndExit();
    }
    gr_clear(0xFF);
    HLock(gPalHdl);
    gr_set_pal(0, 256, (uint8_t *)*gPalHdl);
    HUnlock(gPalHdl);
}

//------------------------------------------------------------------------
//  Create a new movie file, and get the movie ready to add video samples.
//------------------------------------------------------------------------
void CreateAMovie(void)
{
    OSErr            result;
    Point            dlgPos = {120,120};
    SFReply            sfr;
    FSSpec            mySpec;
    Str255            name = "\pOutput Movie";
    int32_t             maxCompressedFrameSize;

    SFPutFile(dlgPos, "\pSave Movie as:", name, 0L, &sfr);
    if (!sfr.good)
     {
         ExitMovies();
        CleanupAndExit();
     }

    ClearMoviesStickyError();
    FSMakeFSSpec(sfr.vRefNum, 0, sfr.fName, &mySpec);
    result = CreateMovieFile(&mySpec, 'TVOD', 0, createMovieFileDeleteCurFile,
                             &gMovieResNum, &gMovie);
    CheckError(result, "\pCan't create output movie file.");

    SetMovieColorTable(gMovie, gMainColorHand);

    // Add the sound track here.
    //    MyCreateTextTrack(gMovie);

    gTrack = NewMovieTrack(gMovie, 600L<<16, 300L<<16, 0);
    CheckError (GetMoviesError(), "\pNew video track." );

    gMedia = NewTrackMedia(gTrack, VideoMediaType, 30, 0L, 0L);
    CheckError (GetMoviesError(), "\pNew Media for video track." );

    BeginMediaEdits(gMedia);        // We do this since we are adding samples to the media
    GetMaxCompressionSize(((CGrafPort *)(gMainWindow))->portPixMap,
                            &gMovieRect,
                            8, spatialQ, codec, codecType,
                            &maxCompressedFrameSize);

    gCompressedFrameBitsH = NewHandle(maxCompressedFrameSize);
    CheckError(MemError(), "\pCan't allocate output frame buffer.");

    result = CompressSequenceBegin(&gSeq, ((CGrafPort *)(gMainWindow))->portPixMap, 0L,
            &gMovieRect, 0L, 8, codec, codecType,
            spatialQ, temporalQ, 15,
            0L, kPrevious, gImageDescriptionH);
    CheckError(result, "\pCan't begin sequence.");
}

//------------------------------------------------------------------------
//  We're finished writing to the movie, so save the video track and close it down.
//------------------------------------------------------------------------
void EndAMovie(void)
{
    OSErr    result;

    CDSequenceEnd(gSeq);
    EndMediaEdits( gMedia );

    result = InsertMediaIntoTrack(gTrack, 0L, 0L, GetMediaDuration(gMedia), 1L<<16);
    CheckError(result, "\pCan't insert media into track.");

    // Finally, we're done with the movie.
    result = AddMovieResource(gMovie, gMovieResNum, 0L,0L);
    CheckError(result, "\pCan't add the movie resource.");

    CloseMovieFile( gMovieResNum );

    if (gCompressedFrameBitsH)
        DisposeHandle(gCompressedFrameBitsH);

    if ( gMovie )
        DisposeMovie(gMovie);
}


//    ===============================================================
//    QuickTime file dumping routines.
//    ===============================================================

//    --------------------------------------------------------------
//
//    QuikReadChunkHdr() reads in the next chunk header, returns true if ok.

bool QuikReadChunkHdr(FILE *fp, QT_ChunkHdr *phdr)
{
    fread(phdr, sizeof(QT_ChunkHdr), 1, fp);

    switch (phdr->ctype)
    {
        case QT_TRAK:
            currTrackType = TRACK_OTHER;
            break;

        case QT_VMHD:
            currTrackType = TRACK_VIDEO;
            break;

        case QT_SMHD:
            currTrackType = TRACK_AUDIO;
            break;
    }

    return(feof(fp) == 0);
}

//    --------------------------------------------------------------
//
//    QuikFindChunkInfo() finds info for a chunk.

QT_ChunkInfo *QuikFindChunkInfo(QT_ChunkHdr *phdr)
{
static QT_Ctype lastType = 0;
static QT_ChunkInfo *lastInfoPtr = NULL;

    QT_ChunkInfo *pinfo;

    if (lastType == phdr->ctype)
        return((QT_ChunkInfo *) lastInfoPtr);

    pinfo = chunkInfo;
    while (pinfo->ctype)
        {
        if (pinfo->ctype == phdr->ctype)
            {
            lastType = phdr->ctype;
            lastInfoPtr = pinfo;
            return((QT_ChunkInfo *) pinfo);
            }
        ++pinfo;
        }
    return NULL;
}

//    --------------------------------------------------------------
//
//    QuikSkipChunk() skips over the data in the current chunk.

void QuikSkipChunk(FILE *fp, QT_ChunkHdr *phdr)
{
    fseek(fp, phdr->length - sizeof(QT_ChunkHdr), SEEK_CUR);
}



//================================================================
//    QuickTime Sound Track routines.
//================================================================

#define    kSoundSampleDuration      1
#define    kSyncSample             0
#define    kTrackStart                0
#define    kMediaStart                0
#define    kFix1                    0x00010000

//----------------------------------------------------------------
void CreateMySoundTrack(Movie theMovie)
{
    Track                     theTrack;
    Media                     theMedia;
    Handle                    sndHandle = nil;
    SoundDescriptionHandle    sndDesc = nil;
    int32_t                     sndDataOffset;
    int32_t                     sndDataSize;
    int32_t                     numSamples;
    OSErr                    err = noErr;

    sndHandle = GetIndResource ('snd ', 1);
    CheckError (ResError(), "\pGetResource 'snd '" );
    if (sndHandle == nil) return;

    sndDesc = (SoundDescriptionHandle)NewHandle(4);
    CheckError (MemError(), "\pNewHandle for SoundDesc" );

    CreateSoundDescription (sndHandle, sndDesc, &sndDataOffset, &numSamples, &sndDataSize );

    theTrack = NewMovieTrack (theMovie, 0, 0, kFullVolume);
    CheckError (GetMoviesError(), "\pNew Sound Track" );

    theMedia = NewTrackMedia (theTrack, SoundMediaType, FixRound ((**sndDesc).sampleRate), nil, 0);
    CheckError (GetMoviesError(), "\pNew Media snd." );

    err = BeginMediaEdits (theMedia);
    CheckError( err, "\pBeginMediaEdits snd." );

    err = AddMediaSample(theMedia, sndHandle, sndDataOffset, sndDataSize,    1,
                            (SampleDescriptionHandle) sndDesc, numSamples, 0, nil);
    CheckError( err, "\pAddMediaSample snd." );

    err = EndMediaEdits (theMedia);
    CheckError( err, "\pEndMediaEdits snd." );

    err = InsertMediaIntoTrack (theTrack, 0, 0, GetMediaDuration (theMedia), kFix1);
    CheckError( err, "\pInsertMediaIntoTrack snd." );

    if (sndDesc != nil) DisposeHandle( (Handle)sndDesc);
}

//----------------------------------------------------------------
void CreateSoundDescription(Handle sndHandle, SoundDescriptionHandle    sndDesc,
                            int32_t *sndDataOffset, int32_t *numSamples, int32_t *sndDataSize)
{
    int32_t                    sndHdrOffset = 0;
    int32_t                    sampleDataOffset;
    SoundHeaderPtr         sndHdrPtr = nil;
    int32_t                    numFrames;
    int32_t                    samplesPerFrame;
    int32_t                    bytesPerFrame;
    SignedByte            sndHState;
    SoundDescriptionPtr    sndDescPtr;

    *sndDataOffset = 0;
    *numSamples = 0;
    *sndDataSize = 0;

    SetHandleSize( (Handle)sndDesc, sizeof(SoundDescription) );
    CheckError(MemError(),"\pSetHandleSize for sndDesc.");

    sndHdrOffset = GetSndHdrOffset (sndHandle);
    if (sndHdrOffset == 0) CheckError(-1,  "\pGetSndHdrOffset ");

    // we can use pointers since we don't move memory
    sndHdrPtr = (SoundHeaderPtr)(*sndHandle + sndHdrOffset);
    sndDescPtr = *sndDesc;
    sndDescPtr->descSize = sizeof (SoundDescription);            // total size of sound desc structure
    sndDescPtr->resvd1 = 0;
    sndDescPtr->resvd2 = 0;
    sndDescPtr->dataRefIndex = 1;
    sndDescPtr->compressionID = 0;
    sndDescPtr->packetSize = 0;
    sndDescPtr->version = 0;
    sndDescPtr->revlevel = 0;
    sndDescPtr->vendor = 0;

    switch  (sndHdrPtr->encode)
    {
        case stdSH:
            sndDescPtr->dataFormat = 'raw ';                        // uncompressed offset-binary data
            sndDescPtr->numChannels = 1;                        // number of channels of sound
            sndDescPtr->sampleSize = 8;                            // number of bits per sample
            sndDescPtr->sampleRate = sndHdrPtr->sampleRate;    // sample rate
            *numSamples = sndHdrPtr->length;
            *sndDataSize = *numSamples;
            bytesPerFrame = 1;
            samplesPerFrame = 1;
            sampleDataOffset = (Ptr)&sndHdrPtr->sampleArea - (Ptr)sndHdrPtr;
            break;

        case extSH:
        {
            ExtSoundHeaderPtr    extSndHdrP = (ExtSoundHeaderPtr)sndHdrPtr;

            sndDescPtr->dataFormat = 'raw ';                            // uncompressed offset-binary data
            sndDescPtr->numChannels = extSndHdrP->numChannels;    // number of channels of sound
            sndDescPtr->sampleSize = extSndHdrP->sampleSize;        // number of bits per sample
            sndDescPtr->sampleRate = extSndHdrP->sampleRate;         // sample rate
            numFrames = extSndHdrP->numFrames;
            *numSamples = numFrames;
            bytesPerFrame = extSndHdrP->numChannels * ( extSndHdrP->sampleSize / 8);
            samplesPerFrame = 1;
            *sndDataSize = numFrames * bytesPerFrame;
            sampleDataOffset = (Ptr)(&extSndHdrP->sampleArea) - (Ptr)extSndHdrP;
        }
            break;

        default:
            CheckError(-1, "\pCorrupt sound data or unsupported format." );
            break;

    }
    *sndDataOffset = sndHdrOffset + sampleDataOffset;
}

//----------------------------------------------------------------
typedef SndCommand *SndCmdPtr;

typedef struct
{
    int16_t             format;
    int16_t             numSynths;
} Snd1Header, *Snd1HdrPtr, **Snd1HdrHndl;

typedef struct
{
    int16_t             format;
    int16_t             refCount;
} Snd2Header, *Snd2HdrPtr, **Snd2HdrHndl;
typedef struct
{
    int16_t             synthID;
    int32_t             initOption;
} SynthInfo, *SynthInfoPtr;


int32_t GetSndHdrOffset (Handle sndHandle)
{
    int16_t    howManyCmds;
    int32_t        sndOffset  = 0;
    Ptr        sndPtr;

    if (sndHandle == nil) return 0;
    sndPtr = *sndHandle;
    if (sndPtr == nil) return 0;

    if ((*(Snd1HdrPtr)sndPtr).format == firstSoundFormat)
    {
        int16_t synths = ((Snd1HdrPtr)sndPtr)->numSynths;
        sndPtr += sizeof(Snd1Header) + (sizeof(SynthInfo) * synths);
    }
    else
    {
        sndPtr += sizeof(Snd2Header);
    }

    howManyCmds = *(int16_t *)sndPtr;

    sndPtr += sizeof(howManyCmds);

    // sndPtr is now at the first sound command--cruise all
    // commands and find the first soundCmd or bufferCmd

    while (howManyCmds > 0)
    {
        switch (((SndCmdPtr)sndPtr)->cmd)
        {
            case (soundCmd + dataOffsetFlag):
            case (bufferCmd + dataOffsetFlag):
                sndOffset = ((SndCmdPtr)sndPtr)->param2;
                howManyCmds = 0;                            // done, get out of loop
                break;
            default:                                            // catch any other type of commands
                sndPtr += sizeof(SndCommand);
                howManyCmds--;
                break;
        }
    }

    return sndOffset;
}

/*    DEATH MOVIE

    // English.
    int8_t                    theText1[] = "They find your body and give it new life.";
    int8_t                    theText2[] = "As a cyborg, you will serve SHODAN well.";
    // German.
    int8_t                    theText1[] = "Sie finden deine Leiche und geben ihr neues Leben.";
    int8_t                    theText2[] = "Als Cyborg wirst du SHODAN treu dienen.";
    // French.
    int8_t                    theText1[] = "Ils ont trouv?? ton corps et lui ont redonn?? la vie.";
    int8_t                    theText2[] = "En tant que cyborgue, tu serviras bien SHODAN.";

    // Timing - 507 total
    blank    9
    text 1    150
    blank    17
    text 2    331
*/

/*    ENDGAME MOVIE

    // English.
    int8_t                    theText1[] = "It's over.";
    int8_t                    theText2[] = "They offered you a nice, boring job at Trioptimum; it never occured to you to take it.";
    int8_t                    theText3[] = "Old habits die hard.";
    // German.
    int8_t                    theText1[] = "Es ist vorbei.";
    int8_t                    theText2[] = "Sie haben dir einen netten Job bei Triop angeboten; es w??re dir nie eingefallen, ihr Angebot anzunehmen.";
    int8_t                    theText3[] = "Du kannst es eben einfach nicht lassen.";
    // French.
    int8_t                    theText1[] = "C'est fini.";
    int8_t                    theText2[] = "Ils vous ont offert un bon boulot ennuyeux sur Triop; ??a ne vous est jamais venu ?? l'id??e de l'accepter.";
    int8_t                    theText3[] = "On a du mal ?? se d??barasser des mauvaises habitudes.";

    // Timing - 507 total
    blank    9
    text 1    180
    blank    17
    text 2    176
    blank    30
    text 2    1021
*/
void MyCreateTextTrack(Movie theMovie)
{
    Track                     theTrack;
    Media                     theMedia;
    OSErr                    err = noErr;
    int8_t                    blankText[] = " ";
    int8_t                    theText1[] = "Ils ont trouv?? ton corps et lui ont redonn?? la vie.";
    int8_t                    theText2[] = "En tant que cyborgue, tu serviras bien SHODAN.";
    RGBColor                black = {0, 0, 0};
    RGBColor                white = {0xeeee, 0xeeee, 0xeeee};
    Rect                    textBox;
    TimeValue                retTime;

    SetRect(&textBox, 0, 320, 600, 350);
    theTrack = NewMovieTrack(theMovie, 600L<<16, 350L<<16, 0);
    CheckError (GetMoviesError(), "\pNew text track." );

    theMedia = NewTrackMedia(theTrack, TextMediaType, 30, 0L, 0L);
    CheckError (GetMoviesError(), "\pNew Media for text track." );

    err = BeginMediaEdits (theMedia);
    CheckError( err, "\pBeginMediaEdits: text." );

    err = AddTextSample(GetMediaHandler(theMedia), (Ptr)blankText, strlen(blankText), geneva, 14, bold,
                         &white, &black, teCenter, &textBox, 0, 0, 0, 0, nil, 9, &retTime);
    CheckError( err, "\pAddTextSample 1." );
    err = AddTextSample(GetMediaHandler(theMedia), (Ptr)theText1, strlen(theText1), geneva, 14, bold,
                         &white, &black, teCenter, &textBox, 0, 0, 0, 0, nil, 150, &retTime);
    CheckError( err, "\pAddTextSample 2." );
    err = AddTextSample(GetMediaHandler(theMedia), (Ptr)blankText, strlen(blankText), geneva, 14, bold,
                         &white, &black, teCenter, &textBox, 0, 0, 0, 0, nil, 17, &retTime);
    CheckError( err, "\pAddTextSample 3." );
    err = AddTextSample(GetMediaHandler(theMedia), (Ptr)theText2, strlen(theText2), geneva, 14, bold,
                         &white, &black, teCenter, &textBox, 0, 0, 0, 0, nil, 331, &retTime);
    CheckError( err, "\pAddTextSample 4." );

    err = EndMediaEdits(theMedia);
    CheckError( err, "\pEndMediaEdits: text." );

    err = InsertMediaIntoTrack(theTrack, 0, 0, GetMediaDuration(theMedia), kFix1);
    CheckError( err, "\pInsertMediaIntoTrack: text." );
}
