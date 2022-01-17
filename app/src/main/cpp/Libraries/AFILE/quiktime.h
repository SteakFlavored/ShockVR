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
//		QUIKTIME.H		QuickTime file access
//		Rex E. Bradford

/*
 * $Source: r:/prj/lib/src/afile/RCS/quiktime.h $
 * $Revision: 1.4 $
 * $Author: rex $
 * $Date: 1994/10/04 20:31:57 $
 * $Log: quiktime.h $
 * Revision 1.4  1994/10/04  20:31:57  rex
 * Added depth16 flag and macro to set it
 *
 * Revision 1.3  1994/09/30  16:57:34  rex
 * Added stuff for writing quicktime movies
 *
 * Revision 1.2  1994/09/29  10:34:57  rex
 * Put globals into a single struct, revamped prototypes, added writing
 *
 * Revision 1.1  1994/09/27  17:23:17  rex
 * Initial revision
 *
*/

#ifndef __QUIKTIME_H
#define __QUIKTIME_H

#include <stdio.h>

#ifndef __TYPES_H
#include "lg_types.h"
#endif
#ifndef __FIX_H
#include "fix.h"
#endif
#ifndef __2D_H
#include "2d.h"
#endif

//	Data types

typedef int16_t fix8;		// 8.8 fixed-point number
typedef uint32_t QT_Ctype;	// quicktime chunk type

typedef struct {
	uint32_t length;			// chunk length
	QT_Ctype ctype;		// chunk type
} QT_ChunkHdr;

typedef struct {
	uint32_t ctype;			// chunk type
	bool isleaf;			// is leaf chunk, or are there subchunks
} QT_ChunkInfo;

typedef struct {
	uint8_t len;				// pascal strings start with length byte
	int8_t str[1];			// followed by string
} PStr;

//	4-char chunk mnemonics

#define MAKE4(c0,c1,c2,c3) ((((uint32_t)c0)<<24)|(((uint32_t)c1)<<16)|(((uint32_t)c2)<<8)|((uint32_t)c3))

#define QT_CLIP	MAKE4('c','l','i','p')
#define QT_CRGN	MAKE4('c','r','g','n')
#define QT_DINF	MAKE4('d','i','n','f')
#define QT_DREF	MAKE4('d','r','e','f')
#define QT_EDTS	MAKE4('e','d','t','s')
#define QT_ELST	MAKE4('e','l','s','t')
#define QT_HDLR	MAKE4('h','d','l','r')
#define QT_KMAT	MAKE4('k','m','a','t')
#define QT_MATT	MAKE4('m','a','t','t')
#define QT_MDAT	MAKE4('m','d','a','t')
#define QT_MDIA	MAKE4('m','d','i','a')
#define QT_MDHD	MAKE4('m','d','h','d')
#define QT_MINF	MAKE4('m','i','n','f')
#define QT_MOOV	MAKE4('m','o','o','v')
#define QT_MVHD	MAKE4('m','v','h','d')
#define QT_SMHD	MAKE4('s','m','h','d')
#define QT_STBL	MAKE4('s','t','b','l')
#define QT_STCO	MAKE4('s','t','c','o')
#define QT_STSC	MAKE4('s','t','s','c')
#define QT_STSD	MAKE4('s','t','s','d')
#define QT_STSH	MAKE4('s','t','s','h')
#define QT_STSS	MAKE4('s','t','s','s')
#define QT_STSZ	MAKE4('s','t','s','z')
#define QT_STTS	MAKE4('s','t','t','s')
#define QT_TKHD	MAKE4('t','k','h','d')
#define QT_TRAK	MAKE4('t','r','a','k')
#define QT_UDTA	MAKE4('u','d','t','a')
#define QT_VMHD	MAKE4('v','m','h','d')

//	Auxiliary structures

//typedef struct {
//	uint8_t len;
//	uint8_t str[31];
//} Str31;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
} QTS_STSD_Base;

typedef struct {
	uint32_t descSize;
	uint32_t dataFormat;
	uint8_t reserved1[6];
	int16_t dataRefIndex;
	int16_t version;
	int16_t revLevel;
	uint32_t vendor;
	int16_t numChans;
	int16_t sampSize;
	int16_t compId;
	int16_t packetSize;
	fix sampRate;
} QT_SoundDesc;

typedef struct {
	uint32_t descSize;
	uint32_t dataFormat;
	uint8_t reserved1[6];
	int16_t dataRefIndex;
	int16_t version;
	int16_t revLevel;
	uint32_t vendor;
	uint32_t temporalQuality;
	uint32_t spatialQuality;
	int16_t width;
	int16_t height;
	fix hRes;
	fix vRes;
	int32_t dataSize;
	int16_t frameCount;
	Str31 name;
	int16_t depth;
	int16_t clutId;
} QT_ImageDesc;

typedef struct {
	uint32_t descSize;
	uint32_t dataFormat;
	// more stuff, but who cares?
} QT_TextDesc;

typedef struct {
	uint32_t trackDuration;
	int32_t mediaTime;
	fix mediaRate;
} QT_EditList;

typedef struct {
	uint32_t count;
	uint32_t duration;
} QT_Time2Samp;

typedef struct {
	uint32_t firstChunk;
	uint32_t sampsPerChunk;
	uint32_t sampDescId;
} QT_Samp2Chunk;

typedef struct {
	uint32_t frameDiffSampNum;
	uint32_t syncSampNum;
} QT_ShadowSync;

//	Chunk structures

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
	QT_EditList editList[1];
} QTS_ELST;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t compType;
	uint32_t compSubtype;
	uint32_t compManufacturer;
	uint32_t compFlags;
	uint32_t compFlagsMask;
	PStr compName;
} QTS_HDLR;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t createTime;
	uint32_t modTime;
	uint32_t timeScale;
	uint32_t duration;
	int16_t language;
	int16_t quality;
} QTS_MDHD;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t createTime;
	uint32_t modTime;
	uint32_t timeScale;
	uint32_t duration;
	fix preferredRate;
	fix8 preferredVol;
	uint8_t reserved[10];
	fix matrix[9];
	uint32_t previewTime;
	uint32_t previewDur;
	uint32_t posterTime;
	uint32_t selTime;
	uint32_t selDur;
	uint32_t currTime;
	uint32_t nextTrackId;
} QTS_MVHD;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	int16_t balance;
	int16_t reserved;
} QTS_SMHD;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
	uint32_t offset[1];
} QTS_STCO;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
	QT_Samp2Chunk samp2chunk[1];
} QTS_STSC;

typedef struct {
	QTS_STSD_Base base;
	union {
		QT_SoundDesc sdesc;
		QT_ImageDesc idesc;
		QT_TextDesc tdesc;
		};
} QTS_STSD;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
	QT_ShadowSync shadowSync[1];
} QTS_STSH;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
	uint32_t sample[1];
} QTS_STSS;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t sampSize;
	uint32_t numEntries;
	uint32_t sampSizeTab[1];
} QTS_STSZ;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t numEntries;
	QT_Time2Samp time2samp[1];
} QTS_STTS;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	uint32_t createTime;
	uint32_t modTime;
	uint32_t trackId;
	uint8_t reserved1[4];
	uint32_t duration;
	uint8_t reserved2[8];
	int16_t layer;
	int16_t altGroup;
	fix8 volume;
	uint8_t reserved3[2];
	fix matrix[9];
	fix trackWidth;
	fix trackHeight;
} QTS_TKHD;

typedef struct {
	uint8_t version;
	uint8_t flags[3];
	int16_t graphicsMode;
	uint16_t opColor[3];
} QTS_VMHD;

//	Quicktime movie structures for reading and writing whole movies

typedef enum {TRACK_VIDEO,TRACK_AUDIO,TRACK_OTHER} TrackType;

typedef struct {
	QTS_TKHD	qt_tkhd;		// track header (TKHD)
	QTS_MDHD qt_mdhd;		// media header (MDHD)
	QTS_STSD qt_stsd;		// sample descriptor (STSD)
	QTS_STTS *qt_stts;	// ptr to time->sample table (STTS)
	QTS_STSC *qt_stsc;	// ptr to sample->chunk table (STSC)
	QTS_STSZ *qt_stsz;	// ptr to sample size table (STSZ)
	QTS_STCO *qt_stco;	//	ptr to chunk->offset table (STCO)

	uint8_t *palette;		// 256-entry palette or NULL
	uint32_t numSamps;		// number of samples
	uint8_t *sampBuff;		// sample buffer (if NULL, then in file)
	fix *sampTime;			// array of sample->time
	uint32_t *sampSize;		// array of sample->size
	uint32_t *sampOffset;	// array of sample->fileoffset
	uint32_t audioBlockSize;	// # bytes per audio sample block
	TrackType type;		// TRACK_XXX
//	The following only used when writing movies
	int16_t numSamplesAlloced;	// # sample entries allocated in buffs
	fix *pSampleTime;		// ptr to array of sample times
} MovieTrack;

typedef struct {			// info about current track, have we gotten
	bool gotTKHD;			// these important chunks? (TKHD, MDHD, STBL)
	bool gotMDHD;
	bool gotSTBL;
} MovieTrackStatus;

#define QTM_MAX_TRACKS 8			// hey, what's in this thing anyway??
#define QTM_MAX_CHUNK_NESTING 8	// max nesting of subchunks

typedef struct {
	QTS_MVHD qt_mvhd;			// global movie header
	MovieTrack track[QTM_MAX_TRACKS];	// here are all my tracks!
	MovieTrack *pVideoTrack;	// ptr to single video track, or NULL if no video
	MovieTrack *pAudioTrack;	// ptr to audio track, or NULL if no audio
	int32_t numTracks;					// number of tracks read into track[] array
	int32_t numAudioSamplesPerBlock;	// 4K if 11Khz, 8K if 22 Khz
	uint32_t compTypeQT;				// type of QuickTime VIDEO compression
	int16_t frameWidth;				// frame width of video track
	int16_t frameHeight;			// frame height of video track
	uint8_t *pFrameCurr;			// current frame in flat format
	uint8_t *pFrameCompQT;			// current frame in compressed format
//	The following used only when writing movies
	int32_t offsetStack[QTM_MAX_CHUNK_NESTING];	// stack of lengths to update
	int16_t indexOffsetStack;		// current index into offset stack
	bool depth16;					// convert to 16-bit depth when writing
} QTM;

//	Prototypes: quiktime.c (reading movie for processing/conversion)

void QuikReadMovie(QTM *pqtm, FILE *fpi);
void QuikFreeMovie(QTM *pqtm);
fix QuikComputeFrameRate(QTM *pqtm);
void *QuikGetVideoSample(QTM *pqtm, int32_t isample, FILE *fpi, int32_t *plength,
	uint8_t *pbmtype, fix *ptime);
void *QuikGetAudioSample(QTM *pqtm, int32_t isample, int32_t *plength, fix *ptime);

//	Protoypes: quikwrite.c (writing movie)

void QuikCreateMovie(QTM *pqtm, FILE *fp);
void QuikSetPal(QTM *pqtm, uint8_t *pal);
void QuikAddVideoSample(QTM *pqtm, FILE *fp, grs_bitmap *pbm, fix time);
void QuikWriteMovieAndClose(QTM *pqtm, FILE *fp);

#define QuikSetDepth16(pqtm) ((pqtm)->depth16 = true)

//	Prototypes: quikconv.c (reading chunks for inspection/dumping)

FILE *QuikOpenFile(int8_t *filename);
bool QuikReadChunkHdr(FILE *fp, QT_ChunkHdr *phdr);
void QuikSkipChunk(FILE *fp, QT_ChunkHdr *phdr);
bool QuikReadChunk(FILE *fp, QT_ChunkHdr *phdr, void *buff, uint32_t bufflen);
void QuikWriteChunkHdr(FILE *fp, QT_ChunkHdr chunkHdr);
void QuikWriteChunkLength(FILE *fp, int32_t length);
void QuikWriteChunk(FILE *fp, QT_Ctype ctype, void *data, uint32_t len);
QT_ChunkInfo *QuikFindChunkInfo(QT_ChunkHdr *phdr);

#define Flip4(v) Flip4Func((uint32_t *)(v))
#define Flip2(v) Flip2Func((uint16_t *)(v))

void Flip4Func(uint32_t *pval4);
void Flip2Func(uint16_t *pval2);

//	Prototypes: quikprnt.c (printing chunks gotten via quikconv.c)

void QuikPrintChunk(QT_ChunkHdr *phdr, void *buff, int8_t *indent);

#endif

