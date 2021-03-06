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
//        QUIKCONV.C        QuickTime file chunk reading and conversion
//        Rex E. Bradford

/*
 * $Source: r:/prj/lib/src/afile/RCS/quikconv.c $
 * $Revision: 1.2 $
 * $Author: rex $
 * $Date: 1994/09/30 16:59:24 $
 * $Log: quikconv.c $
 * Revision 1.2  1994/09/30  16:59:24  rex
 * Modified conversion routines to support writing too
 *
 * Revision 1.1  1994/09/27  17:22:58  rex
 * Initial revision
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#include <dbg.h>
//#include <fname.h>
#include "quiktime.h"

typedef struct {        // THIS STRUCT MUST MATCH QT_ChunkInfo IN FIRST VARS
    uint32_t ctype;
    bool isleaf;
    void (*f_convert)(void *data, uint32_t length, bool read);
} QT_ChunkInfoAndConvert;

void ConvertELST(void *data, uint32_t length, bool read);
void ConvertHDLR(void *data, uint32_t length, bool read);
void ConvertMDHD(void *data, uint32_t length, bool read);
void ConvertMVHD(void *data, uint32_t length, bool read);
void ConvertSMHD(void *data, uint32_t length, bool read);
void ConvertSTCO(void *data, uint32_t length, bool read);
void ConvertSTSC(void *data, uint32_t length, bool read);
void ConvertSTSD(void *data, uint32_t length, bool read);
void ConvertSTSH(void *data, uint32_t length, bool read);
void ConvertSTSS(void *data, uint32_t length, bool read);
void ConvertSTSZ(void *data, uint32_t length, bool read);
void ConvertSTTS(void *data, uint32_t length, bool read);
void ConvertTKHD(void *data, uint32_t length, bool read);
void ConvertVMHD(void *data, uint32_t length, bool read);

QT_ChunkInfoAndConvert chunkInfo[] = {
    QT_CLIP,false,NULL,
    QT_CRGN,true,NULL,
    QT_DINF,false,NULL,
    QT_DREF,true,NULL,
    QT_EDTS,false,NULL,
    QT_ELST,true,ConvertELST,
    QT_HDLR,true,ConvertHDLR,
    QT_KMAT,true,NULL,
    QT_MATT,false,NULL,
    QT_MDAT,true,NULL,
    QT_MDIA,false,NULL,
    QT_MDHD,true,ConvertMDHD,
    QT_MINF,false,NULL,
    QT_MOOV,false,NULL,
    QT_MVHD,true,ConvertMVHD,
    QT_SMHD,true,ConvertSMHD,
    QT_STBL,false,NULL,
    QT_STCO,true,ConvertSTCO,
    QT_STSC,true,ConvertSTSC,
    QT_STSD,true,ConvertSTSD,
    QT_STSH,true,ConvertSTSH,
    QT_STSS,true,ConvertSTSS,
    QT_STSZ,true,ConvertSTSZ,
    QT_STTS,true,ConvertSTTS,
    QT_TKHD,true,ConvertTKHD,
    QT_TRAK,false,NULL,
    QT_UDTA,false,NULL,
    QT_VMHD,true,ConvertVMHD,
    0,0,NULL,
};

TrackType currTrackType;

/*
//    --------------------------------------------------------------
//        PUBLIC FUNCTIONS
//    --------------------------------------------------------------
//
//    QuikOpenFile() attempts to open a quicktime file for reading.

FILE *QuikOpenFile(int8_t *filename)
{
    FILE *fp;
    Fname fname;

    FnameExtract(&fname, filename);
    FnameAddExt(&fname, "qtm");
    FnameBuild(filename, &fname);
    fp = fopen(filename, "rb");
    return(fp);
}

//    --------------------------------------------------------------
//
//    QuikReadChunkHdr() reads in the next chunk header, returns true if ok.

bool QuikReadChunkHdr(FILE *fp, QT_ChunkHdr *phdr)
{
    fread(phdr, sizeof(QT_ChunkHdr), 1, fp);
    Flip4(&phdr->length);
    Flip4(&phdr->ctype);

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
//    QuikSkipChunk() skips over the data in the current chunk.

void QuikSkipChunk(FILE *fp, QT_ChunkHdr *phdr)
{
    fseek(fp, phdr->length - sizeof(QT_ChunkHdr), SEEK_CUR);
}

//    ----------------------------------------------------------------
//
//    QuikReadChunk() reads data in chunk.

bool QuikReadChunk(FILE *fp, QT_ChunkHdr *phdr, void *buff, uint32_t bufflen)
{
    QT_ChunkInfoAndConvert *pinfo;

    if ((phdr->length - sizeof(QT_ChunkHdr)) > bufflen)
        {
        Warning("QuikReadChunk: chunk too big!\n");
        return false;
        }

    fread(buff, phdr->length - sizeof(QT_ChunkHdr), 1, fp);

    pinfo = (QT_ChunkInfoAndConvert *) QuikFindChunkInfo(phdr);
    if (pinfo && pinfo->f_convert)
        (*pinfo->f_convert)(buff, phdr->length - sizeof(QT_ChunkHdr), true);
    return true;
}
*/
//    --------------------------------------------------------------
//
//    QuikWriteChunkHdr() writes a chunk header.

void QuikWriteChunkHdr(FILE *fp, QT_ChunkHdr chunkHdr)
{
    Flip4(&chunkHdr.length);
    Flip4(&chunkHdr.ctype);
    fwrite(&chunkHdr, sizeof(chunkHdr), 1, fp);
}

//    --------------------------------------------------------------
//
//    QuikWriteChunkLength() writes a chunk length.

void QuikWriteChunkLength(FILE *fp, int32_t length)
{
    Flip4(&length);
    fwrite(&length, sizeof(int32_t), 1, fp);
}

//    --------------------------------------------------------------
//
//    QuikWriteChunk() writes a chunk.

void QuikWriteChunk(FILE *fp, QT_Ctype ctype, void *data, uint32_t len)
{
    QT_ChunkInfoAndConvert *pinfo;
    QT_ChunkHdr chunkHdr;

    chunkHdr.length = len + sizeof(chunkHdr);
    chunkHdr.ctype = ctype;
    QuikWriteChunkHdr(fp, chunkHdr);

    if (len)
        {
        pinfo = (QT_ChunkInfoAndConvert *) QuikFindChunkInfo(&chunkHdr);
        if (pinfo && pinfo->f_convert)
            (*pinfo->f_convert)(data, len, false);
        fwrite(data, len, 1, fp);
        }
}

//    --------------------------------------------------------------
//
//    QuikFindChunkInfo() finds info for a chunk.

QT_ChunkInfo *QuikFindChunkInfo(QT_ChunkHdr *phdr)
{
static QT_Ctype lastType = 0;
static QT_ChunkInfoAndConvert *lastInfoPtr = NULL;

    QT_ChunkInfoAndConvert *pinfo;

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

//    ----------------------------------------------------------------
//        CONVERTER ROUTINES
//    ----------------------------------------------------------------

void ConvertELST(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_ELST *p = (QTS_ELST *)data;

    if (read)
        Flip4(&p->numEntries);
    for (i = 0; i < p->numEntries; i++)
        {
        Flip4(&p->editList[i].trackDuration);
        Flip4(&p->editList[i].mediaTime);
        Flip4(&p->editList[i].mediaRate);
        }
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertHDLR(void *data, uint32_t length, bool read)
{
    QTS_HDLR *p = (QTS_HDLR *)data;

    Flip4(&p->compType);
    Flip4(&p->compSubtype);
    Flip4(&p->compManufacturer);
    Flip4(&p->compFlags);
    Flip4(&p->compFlagsMask);
}

void ConvertMDHD(void *data, uint32_t length, bool read)
{
    QTS_MDHD *p = (QTS_MDHD *)data;

    Flip4(&p->createTime);
    Flip4(&p->modTime);
    Flip4(&p->timeScale);
    Flip4(&p->duration);
    Flip2(&p->language);
    Flip2(&p->quality);
}

void ConvertMVHD(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_MVHD *p = (QTS_MVHD *)data;

    Flip4(&p->createTime);
    Flip4(&p->modTime);
    Flip4(&p->timeScale);
    Flip4(&p->duration);
    Flip4(&p->preferredRate);
    Flip2(&p->preferredVol);
    for (i = 0; i < 9; i++)
        Flip4(&p->matrix[i]);
    Flip4(&p->previewTime);
    Flip4(&p->previewDur);
    Flip4(&p->posterTime);
    Flip4(&p->selTime);
    Flip4(&p->selDur);
    Flip4(&p->currTime);
    Flip4(&p->nextTrackId);
}

void ConvertSMHD(void *data, uint32_t length, bool read)
{
    QTS_SMHD *p = (QTS_SMHD *)data;

    Flip2(&p->balance);
}

void ConvertSTCO(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_STCO *p = (QTS_STCO *)data;

    if (read)
        Flip4(&p->numEntries);
    for (i = 0; i < p->numEntries; i++)
        Flip4(&p->offset[i]);
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertSTSC(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_STSC *p = (QTS_STSC *)data;

    if (read)
        Flip4(&p->numEntries);
    for (i = 0; i < p->numEntries; i++)
        {
        Flip4(&p->samp2chunk[i].firstChunk);
        Flip4(&p->samp2chunk[i].sampsPerChunk);
        Flip4(&p->samp2chunk[i].sampDescId);
        }
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertSTSD(void *data, uint32_t length, bool read)
{
    QTS_STSD *p = (QTS_STSD *)data;

    Flip4(&p->base.numEntries);

    Flip4(&p->sdesc.descSize);
    Flip4(&p->sdesc.dataFormat);

    if (currTrackType == TRACK_AUDIO)
        {
        Flip2(&p->sdesc.dataRefIndex);
        Flip2(&p->sdesc.version);
        Flip2(&p->sdesc.revLevel);
        Flip4(&p->sdesc.vendor);
        Flip2(&p->sdesc.numChans);
        Flip2(&p->sdesc.sampSize);
        Flip2(&p->sdesc.compId);
        Flip2(&p->sdesc.packetSize);
        Flip4(&p->sdesc.sampRate);
        }
    else if (currTrackType == TRACK_VIDEO)
        {
        Flip2(&p->idesc.dataRefIndex);
        Flip2(&p->idesc.version);
        Flip2(&p->idesc.revLevel);
        Flip4(&p->idesc.vendor);
        Flip4(&p->idesc.temporalQuality);
        Flip4(&p->idesc.spatialQuality);
        Flip2(&p->idesc.width);
        Flip2(&p->idesc.height);
        Flip4(&p->idesc.hRes);
        Flip4(&p->idesc.vRes);
        Flip4(&p->idesc.dataSize);
        Flip2(&p->idesc.frameCount);
        Flip2(&p->idesc.depth);
        Flip2(&p->idesc.clutId);
        }
    else
        {
        }
}

void ConvertSTSH(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_STSH *p = (QTS_STSH *)data;

    if (read)
        Flip4(&p->numEntries);
    for (i = 0; i < p->numEntries; i++)
        {
        Flip4(&p->shadowSync[i].frameDiffSampNum);
        Flip4(&p->shadowSync[i].syncSampNum);
        }
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertSTSS(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_STSS *p = (QTS_STSS *)data;

    if (read)
        Flip4(&p->numEntries);
    for (i = 0; i < p->numEntries; i++)
        Flip4(&p->sample[i]);
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertSTSZ(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_STSZ *p = (QTS_STSZ *)data;

    Flip4(&p->sampSize);
    if (read)
        Flip4(&p->numEntries);
    if (p->sampSize == 0)
        {
        for (i = 0; i < p->numEntries; i++)
            Flip4(&p->sampSizeTab[i]);
        }
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertSTTS(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_STTS *p = (QTS_STTS *)data;

    if (read)
        Flip4(&p->numEntries);
    for (i = 0; i < p->numEntries; i++)
        {
        Flip4(&p->time2samp[i].count);
        Flip4(&p->time2samp[i].duration);
        }
    if (!read)
        Flip4(&p->numEntries);
}

void ConvertTKHD(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_TKHD *p = (QTS_TKHD *)data;

    Flip4(&p->createTime);
    Flip4(&p->modTime);
    Flip4(&p->trackId);
    Flip4(&p->duration);
    Flip2(&p->layer);
    Flip2(&p->altGroup);
    Flip2(&p->volume);
    for (i = 0; i < 9; i++)
        Flip4(&p->matrix[i]);
    Flip4(&p->trackWidth);
    Flip4(&p->trackHeight);
}

void ConvertVMHD(void *data, uint32_t length, bool read)
{
    int32_t i;
    QTS_VMHD *p = (QTS_VMHD *)data;

    Flip2(&p->graphicsMode);
    for (i = 0; i < 3; i++)
        Flip2(&p->opColor[i]);
}

//    ----------------------------------------------------------------
//        INTERNAL ROUTINES
//    ----------------------------------------------------------------

void Flip4Func(uint32_t *pval4)
{
// For mac version
//    *pval4 = MAKE4(*pval4 & 0xFF,
//        (*pval4 >> 8) & 0xFF,
//        (*pval4 >> 16) & 0xFF,
//        *pval4 >> 24);
}

void Flip2Func(uint16_t *pval2)
{
// For mac version
//    *pval2 = ((*pval2 & 0xFF) << 8) | ((*pval2 >> 8) & 0xFF);
}

