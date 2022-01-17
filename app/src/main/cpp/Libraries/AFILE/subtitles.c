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
//        Subtitles        Extract subtitles from a movie and write them out
//                        to a 'subt' resource.
//
//        Ken Cobb, 2/7/95

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lg.h"
#include "fix.h"

#include "movie.h"

int32_t SwapLongBytes(int32_t in);
int16_t SwapShortBytes(int16_t in);

MovieHeader mh;

// The 'subt' resource is an array of these structs.
typedef struct
{
    int16_t    language;
    int32_t    startingTime;
    Str255    subtitle;
} SubtitleEntry;


//    --------------------------------------------------------------
//        MAIN PROGRAM
//    --------------------------------------------------------------

void main(void)
{
static int8_t *chunkNames[] = {
    "END  ","VIDEO","AUDIO","TEXT ","PAL  ","TABLE","?????","?????"};
static int8_t *bmTypeNames[] = {
    "DEVICE","MONO","FLAT8","FLAT24","RSD8","TLUC8","SPAN","GEN",
    "","","","","","","","4X4",
};
static int8_t *palNames[] = {
    "SET","BLACK","???","???","???","???","???","???",
};
static int8_t *tableNames[] = {
    "COLORSET","HUFFTAB","???","???","???","???","???","???",
    "???","???","???","???","???","???","???","???",
};

    FILE *fpi;
    int32_t iarg;
    int32_t length;
    MovieChunk *pmc,*pmcBase;
    int8_t infile[128];
    int8_t buff[128];

    int16_t    outResNum;
    Handle    subHdl;
    SubtitleEntry    *subPtr;
    int16_t    numSubs = 0;
    int16_t    segment = 0;
    int32_t    segTime = 0;

    printf ("File to extract subtitles: ");
    fgets (infile, sizeof(infile), stdin);
    infile[strlen(infile)-1] = 0;
    if (strchr(infile, '.') == NULL)
        strcat(infile, ".mov");

//    Open input file

    fpi = fopen(infile, "rb");
    if (fpi == NULL)
        {
        printf("Can't open: %s\n", infile);
        exit(1);
        }

// Create and open the output resource file for saving the 'subt' resource into.

    {
        StandardFileReply    reply;
        OSErr                err;

        StandardPutFile("\pText output file:", "\pMovie.Txt", &reply);
        if (!reply.sfGood)
            return;

        if (reply.sfReplacing)                            // Delete the file if it exists.
            FSpDelete(&reply.sfFile);

        FSpCreateResFile(&reply.sfFile, 'Shok', 'rsrc', nil);    // Create the resource file.
        err = ResError();
        if (err != 0)
        {
            printf("Can't create file.\n");
            return;
        }
        outResNum = FSpOpenResFile(&reply.sfFile, fsRdWrPerm);    // Open the file for writing.
        if (outResNum == -1)
        {
            printf("Can't open file.\n");
            return;
        }
    }
    subHdl = NewHandle(sizeof(SubtitleEntry) * 200);    // Allocate a big handle.
    if (!subHdl)
    {
        printf("Can't allocate subtitles handle.\n");
        return;
    }
    HLock(subHdl);
    subPtr = (SubtitleEntry *)*subHdl;

//    Get movie header, check for valid movie file

    fread(&mh, sizeof(mh), 1, fpi);
    if (mh.magicId != MOVI_MAGIC_ID)
        {
        printf("%s not a valid .mov file!\n", infile);
        exit(1);
        }

// Swap all those bytes around.

    mh.numChunks = SwapLongBytes(mh.numChunks);
    mh.sizeChunks = SwapLongBytes(mh.sizeChunks);
    mh.sizeData = SwapLongBytes(mh.sizeData);
    mh.totalTime = SwapLongBytes(mh.totalTime);
    mh.frameRate = SwapLongBytes(mh.frameRate);
    mh.frameWidth = SwapShortBytes(mh.frameWidth);
    mh.frameHeight = SwapShortBytes(mh.frameHeight);
    mh.gfxNumBits = SwapShortBytes(mh.gfxNumBits);
    mh.isPalette = SwapShortBytes(mh.isPalette);
    mh.audioNumChans = SwapShortBytes(mh.audioNumChans);
    mh.audioSampleSize = SwapShortBytes(mh.audioSampleSize);
    mh.audioSampleRate = SwapLongBytes(mh.audioSampleRate);

//    Dump some movie header info

    printf("Movie header:\n");
    fix_sprint (buff, mh.totalTime);
    printf("    total time:      %s\n", buff);
    fix_sprint (buff, mh.frameRate);
    printf("    frame rate:      %s\n\n", buff);

//    Extract the text chunks into a handle.

    pmc = (MovieChunk *)malloc(mh.sizeChunks);
    fread(pmc, mh.sizeChunks, 1, fpi);
      pmcBase = pmc;
    while (true)
    {
        uint8_t    s1, s2;

        // Swap bytes around.
        s1 = *((uint8_t *)pmc);
        s2 = *(((uint8_t *)pmc)+2);
        *(((uint8_t*)pmc)+2) = s1;
        *((uint8_t*)pmc) = s2;
        pmc->offset = SwapLongBytes(pmc->offset);

        // Do this for all chunks, until the end.

        if (pmc->chunkType != MOVIE_CHUNK_END)
        {
                // If text, then extract it.
            if (pmc->chunkType == MOVIE_CHUNK_TEXT)
            {
                MovieTextItem *mti;
                uint32_t     tag;
                int8_t    *cp;

                // Read the text
                fseek(fpi, pmc->offset, SEEK_SET);
                fread(buff, sizeof(buff), 1, fpi);
                mti = (MovieTextItem *)buff;
                tag = mti->tag;
                mti->offset = SwapLongBytes(mti->offset);

                // If it's actually text (not a textitem command)
                if (tag == (uint32_t)'STD ' ||
                    tag == (uint32_t)'FRN ' ||
                    tag == (uint32_t)'GER ')
                {
                    if (tag == (uint32_t)'STD ')                // Place info in struct
                        subPtr->language = 0;
                    else if (tag == (uint32_t)'FRN ')
                        subPtr->language = 1;
                    else if (tag == (uint32_t)'GER ')
                        subPtr->language = 2;
                    subPtr->startingTime = (fix_int(pmc->time) * 600)
                                             + fix_rint(fix_mul(fix_frac(pmc->time),fix_make(600, 0)));
                    BlockMove((int8_t *)mti + mti->offset, subPtr->subtitle, 255);

                    // Replace any linefeeds (0x0A) with carriage returns (0x0D)
                    cp = (int8_t *)subPtr->subtitle;
                    while (*cp)
                    {
                        if (*cp == 0x0A)
                            *cp = 0x0D;
                        cp++;
                    }

                    fix_sprint (infile, pmc->time);            // Print the time
                    printf("time: %s  ", infile);

                    printf("'%c%c%c%c': %s\n",                 // and the text info.
                        tag >> 24, (tag >> 16) & 0xFF, (tag >> 8) & 0xFF, tag & 0xFF,
                        (int8_t *)mti + mti->offset);

                    subPtr++;
                    numSubs++;
                }
            }
          }
        else
        {
            fix_sprint (buff, pmc->time);
            printf("END:      time: %s\n", buff);
        }

        if (pmc->chunkType == MOVIE_CHUNK_END)
            break;
        ++pmc;
    }

// Add a terminating entry to the handle.

    subPtr->language = -1;
    subPtr->startingTime = -1;
    subPtr->subtitle[0] = 0;
    numSubs++;

// Resize subtitles handle, and write it out.

    HUnlock(subHdl);
    SetHandleSize(subHdl, sizeof(SubtitleEntry) * numSubs);

    AddResource(subHdl, 'subt', 128, "\psubtitles");
    WriteResource(subHdl);
    ReleaseResource(subHdl);

//    Close files

    CloseResFile(outResNum);
    fclose(fpi);
}


//    ---------------------------------------------------------
int32_t SwapLongBytes(int32_t in)
{
    int32_t    out;
    *(uint8_t*)&out = *(((uint8_t *)&in)+3);
    *(((uint8_t*)&out)+1) = *(((uint8_t *)&in)+2);
    *(((uint8_t*)&out)+2) = *(((uint8_t *)&in)+1);
    *(((uint8_t*)&out)+3) = *(uint8_t *)&in;
    return(out);
}

int16_t SwapShortBytes(int16_t in)
{
    int16_t out;
    *(uint8_t*)&out = *(((uint8_t *)&in)+1);
    *(((uint8_t*)&out)+1) = *(uint8_t *)&in;
    return(out);
}
