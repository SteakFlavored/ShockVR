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
//        RESBUILD.C        Resource-file building routines
//        Rex E. Bradford (REX)
/*
* $Header: n:/project/lib/src/res/rcs/resbuild.c 1.10 1994/06/16 11:06:30 rex Exp $
* $Log: resbuild.c $
 * Revision 1.10  1994/06/16  11:06:30  rex
 * Got rid of RDF_NODROP flag
 *
 * Revision 1.9  1994/02/17  11:25:32  rex
 * Moved some stuff out to resmake.c and resfile.c
 *
*/

#include <string.h>

#include "res.h"
#include "res_.h"
#include "lzw.h"

#define CTRL_Z 26        // make sure comment ends with one, so can type a file

//    Internal prototypes
bool ResEraseIfInFile(Id id);                // erase item from file

//    -------------------------------------------------------
//
//    ResSetComment() sets comment in res header.
//    -------------------------------------------------------
//    For Mac version:  Does nothing.  May go back later and add comment via the
// desktop database, maybe.

void ResSetComment(int16_t filenum, const char *comment)
{
    ResFileHeader *phead;

    if (resFile[filenum].pedit == NULL) {
        Warning("ResSetComment: file %d not open for writing\n", filenum);
        return;
    }

    phead = &resFile[filenum].pedit->hdr;
    memset(phead->comment, 0, sizeof(phead->comment));
    strncpy(phead->comment, comment, sizeof(phead->comment) - 2);
    phead->comment[strlen(phead->comment)] = CTRL_Z;
}

//    -------------------------------------------------------
//
//    ResWrite() writes a resource to an open resource file.
//    This routine assumes that the file position is already set to
//    the current data position.
//
//        id = id to write
//    -------------------------------------------------------
//    Simply get the resource handle and write it out.  If resource is compressed,
//    do that first before writing.
#define EXTRA 250

int32_t ResWrite(Id id)
{
    static uint8_t pad[] = {0,0,0,0,0,0,0,0};
    ResDesc *prd;
    ResFile *prf;
    ResDirEntry *pDirEntry;
    uint8_t *p;
    int32_t size,sizeTable;
    void *pcompbuff;
    int32_t compsize;
    int32_t padBytes;

    // Check for errors
    if (!ResCheckId(id))
        return -1;

    prd = RESDESC(id);
    prf = &resFile[prd->filenum];
    if (prf->pedit == NULL) {
        Warning("ResWrite: file %d not open for writing\n", prd->filenum);
        return -1;
    }

    // Check if item already in directory, if so erase it
    ResEraseIfInFile(id);

    // If directory full, grow it
    if (prf->pedit->pdir->numEntries == prf->pedit->numAllocDir) {
        prf->pedit->numAllocDir += DEFAULT_RES_GROWDIRENTRIES;
        prf->pedit->pdir = realloc(prf->pedit->pdir,
                sizeof(ResDirHeader) + (sizeof(ResDirEntry) * prf->pedit->numAllocDir));
    }

    // Set resource's file offset
    prd->offset = RES_OFFSET_REAL2DESC(prf->pedit->currDataOffset);

    // Fill in directory entry
    pDirEntry = ((ResDirEntry *) (prf->pedit->pdir + 1)) + prf->pedit->pdir->numEntries;

    pDirEntry->id = id;
    pDirEntry->flags = prd->flags;
    pDirEntry->type = prd->type;
    pDirEntry->size = prd->size;

    // If compound, write out reftable without compression
    lseek(prf->fd, prf->pedit->currDataOffset, SEEK_SET);
    p = prd->ptr;
    sizeTable = 0;
    size = prd->size;
    if (prd->flags & RDF_COMPOUND) {
        sizeTable = REFTABLESIZE(((RefTable *) p)->numRefs);
        write(prf->fd, p, sizeTable);
        p += sizeTable;
        size -= sizeTable;
    }

    // If compression, try it (may not work out)
    if (pDirEntry->flags & RDF_LZW) {
        pcompbuff = malloc(size + EXTRA);
        compsize = LzwCompressBuff2Buff(p, size, pcompbuff, size);
        if (compsize < 0) {
            pDirEntry->flags &= ~RDF_LZW;
        } else {
            pDirEntry->csize = sizeTable + compsize;
            write(prf->fd, pcompbuff, compsize);
        }
        free(pcompbuff);
    }

    // If no compress (or failed to compress well), just write out
    if (!(pDirEntry->flags & RDF_LZW)) {
        pDirEntry->csize = prd->size;
        write(prf->fd, p, size);
    }

    // Pad to align on data boundary
    padBytes = RES_OFFSET_PADBYTES(pDirEntry->csize);
    if (padBytes)
        write(prf->fd, pad, padBytes);

    if (lseek(prf->fd, 0, SEEK_CUR) & 3)
        Warning("ResWrite: misaligned writing!\n");

    // Advance dir num entries, current data offset
    prf->pedit->pdir->numEntries++;
    prf->pedit->currDataOffset = RES_OFFSET_ALIGN(prf->pedit->currDataOffset + pDirEntry->csize);

    return 0;
}

#define SIZE_RESCOPY 32768

static void ResCopyBytes(int32_t fd, int32_t writePos, int32_t readPos, int32_t size)
{
    int32_t sizeCopy;
    uint8_t *buff;

    buff = malloc(SIZE_RESCOPY);

    while (size > 0) {
        sizeCopy = min(SIZE_RESCOPY, size);
        lseek(fd, readPos, SEEK_SET);
        read(fd, buff, sizeCopy);
        lseek(fd, writePos, SEEK_SET);
        write(fd, buff, sizeCopy);
        readPos += sizeCopy;
        writePos += sizeCopy;
        size -= sizeCopy;
    }

    free(buff);
}

//    -------------------------------------------------------------
//
//    ResPack() removes holes from a resource file.
//
//        filenum = resource filenum (must already be open for create/edit)
//
//    Returns: # bytes reclaimed

int32_t ResPack(int32_t filenum)
{
    ResFile *prf;
    ResDirEntry *pDirEntry;
    int32_t numReclaimed,sizeReclaimed;
    int32_t dataRead,dataWrite;
    int32_t i;
    ResDirEntry *peWrite;

    // Check for errors
    prf = &resFile[filenum];
    if (prf->pedit == NULL) {
        Warning("ResPack: filenum %d not open for editing\n", filenum);
        return 0;
    }

    // Set up
    sizeReclaimed = numReclaimed = 0;
    dataRead = dataWrite = prf->pedit->pdir->dataOffset;

    // Scan thru directory, copying over all empty entries
    pDirEntry = (ResDirEntry *) (prf->pedit->pdir + 1);
    for (i = 0; i < prf->pedit->pdir->numEntries; i++) {
        if (pDirEntry->id == 0) {
            numReclaimed++;
            sizeReclaimed += pDirEntry->csize;
        } else {
            if (gResDesc[pDirEntry->id].offset > RES_OFFSET_PENDING)
                gResDesc[pDirEntry->id].offset = RES_OFFSET_REAL2DESC(dataWrite);
            if (dataRead != dataWrite)
                ResCopyBytes(prf->fd, dataWrite, dataRead, pDirEntry->csize);
            dataWrite = RES_OFFSET_ALIGN(dataWrite + pDirEntry->csize);
        }
        dataRead = RES_OFFSET_ALIGN(dataRead + pDirEntry->csize);
        pDirEntry++;
    }

    // Now pack directory itself
    pDirEntry = (ResDirEntry *) (prf->pedit->pdir + 1);
    peWrite = pDirEntry;
    for (i = 0; i < prf->pedit->pdir->numEntries; i++) {
        if (pDirEntry->id) {
            if (pDirEntry != peWrite)
                *peWrite = *pDirEntry;
            peWrite++;
        }
        pDirEntry++;
    }
    prf->pedit->pdir->numEntries -= numReclaimed;

    // Set new current data offset
    prf->pedit->currDataOffset = dataWrite;
    lseek(prf->fd, dataWrite, SEEK_SET);
    prf->pedit->flags &= ~RFF_NEEDSPACK;

    // Return # bytes reclaimed
    return sizeReclaimed;
}

//    --------------------------------------------------------
//        INTERNAL ROUTINES
//    --------------------------------------------------------
//
//    ResEraseIfInFile() erases a resource if it's in a file's directory.
//
//        id = id of item
//
//    Returns: true if found & erased, false otherwise

bool ResEraseIfInFile(Id id)
{
    ResDesc *prd;
    ResFile *prf;
    ResDirEntry *pDirEntry;
    int32_t i;

    prd = RESDESC(id);
    prf = &resFile[prd->filenum];
    pDirEntry = (ResDirEntry *) (prf->pedit->pdir + 1);

    for (i = 0; i < prf->pedit->pdir->numEntries; i++) {
        if (id == pDirEntry->id) {
            pDirEntry->id = 0;
            prf->pedit->flags |= RFF_NEEDSPACK;
            if (prf->pedit->flags & RFF_AUTOPACK)
                ResPack(prd->filenum);
            return true;
        }
        pDirEntry++;
    }

    return false;
}
