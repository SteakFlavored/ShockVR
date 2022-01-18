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
//        ResFile.C        Resource Manager file access
//        Rex E. Bradford (REX)
/*
* $Header: r:/prj/lib/src/res/rcs/resfile.c 1.3 1994/08/07 20:17:31 xemu Exp $
* $Log: resfile.c $
 * Revision 1.3  1994/08/07  20:17:31  xemu
 * generate a warning on resource collision
 *
 * Revision 1.2  1994/06/16  11:07:04  rex
 * Added item to tail of LRU when loadonopen
 *
 * Revision 1.1  1994/02/17  11:23:23  rex
 * Initial revision
 *
*/

#include <string.h>

#include "res.h"
#include "res_.h"

//    Resource files start with this signature
int8_t resFileSignature[16] = {
    'L','G',' ','R','e','s',' ','F','i','l','e',' ','v','2',13,10};

//    The active resource file info table
ResFile resFile[MAX_RESFILENUM+1];

//    Internal prototypes
int32_t ResFindFreeFilenum();
void ResReadDirEntries(int32_t filenum, ResDirHeader *pDirHead);
void ResProcDirEntry(ResDirEntry *pDirEntry, int32_t filenum, int32_t dataOffset);
void ResReadEditInfo(ResFile *prf);
void ResReadDir(ResFile *prf, int32_t filenum);
void ResCreateEditInfo(ResFile *prf, int32_t filenum);
void ResCreateDir(ResFile *prf);
void ResWriteDir(int32_t filenum);
void ResWriteHeader(int32_t filenum);

//    ---------------------------------------------------------
//
//    ResOpenResFile() opens for read/edit/create.
//
//        fname    = ptr to filename
//        mode     = ROM_XXX (see res.h)
//        auxinfo = if true, allocate aux info, including directory
//                        (applies to mode 0, other modes automatically get it)
//
//    Returns:
//
//        -1 = couldn't find free filenum
//        -2 = couldn't open, edit, or create file
//        -3 = invalid resource file
//        -4 = memory allocation failure
//    ---------------------------------------------------------

int32_t ResOpenResFile(const char *filename, ResOpenMode mode, bool auxinfo)
{
    ResFile *prf;
    ResFileHeader fileHeader;
    ResDirHeader dirHeader;
    int32_t filenum;
    int32_t fd = -1;

    // First find a free filenum, otherwise return -1.
    filenum = ResFindFreeFilenum();
    if (filenum < 0) {
        Warning("ResOpenResFile: no free filenum for %s", filename);
        return -1;
    }

    // If any mode but create, open along datapath.  If can't open,
    // return error except if mode 2 (edit/create), in which case
    // drop thru to create case by faking mode 3.
    if (mode != ROM_CREATE) {
        // See if file exists
        fd = open(filename, (mode == ROM_READ) ? O_RDONLY : O_RDWR);
        if (fd < 0) {
            // If it doesn't exist, create the file if requested.
            if (mode == ROM_EDITCREATE) // If this mode, drop through to create the file.
                mode = ROM_CREATE;
            else {
                Warning("ResOpenResFile: can't open file %s", filename);
                return -2;
            }
        }
    }

    // If create mode, or edit/create failed, try to create the file.
    if (mode == ROM_CREATE) {
        fd = open(filename, O_CREAT | O_RDWR, 0644);
        if (fd < 0) {
            Warning("ResOpenResFile: Can't create file %s", filename);
            return -2;
        }
    }

    if (fd < 0) {
        Warning("ResOpenResFile: Failed to open file %s", filename);
        return -2;
    } else {
        read(fd, &fileHeader, sizeof(ResFileHeader));
        if (memcmp(fileHeader.signature, resFileSignature, sizeof(resFileSignature)) != 0) {
            Warning("ResOpenResFile: %s is not valid resource file", filename);
            close(fd);
            return -3;
        }
    }

    // If the file was opened for editing or auxinfo was requested, allocate space for it
    prf = &resFile[filenum];
    prf->pedit = NULL;
    if (mode != ROM_READ || auxinfo) {
        prf->pedit = (ResEditInfo *)malloc(sizeof(ResEditInfo));
        if (prf->pedit == NULL) {
            Warning(("ResOpenResFile: unable to allocate ResEditInfo\n"));
            close(fd);
            return -4;
        }
    }

    prf->fd = fd;

    if (mode == ROM_CREATE) {
        ResCreateEditInfo(prf, filenum);
        ResCreateDir(prf);
    } else {
        // Read in resource map info into the array of resource descriptions
        if (prf->pedit != NULL) {
            ResReadEditInfo(prf);
            ResReadDir(prf, filenum);
            ResPack(filenum);
        } else {
            lseek(fd, fileHeader.dirOffset, SEEK_SET);
            read(fd, &dirHeader, sizeof(ResDirHeader));
            ResReadDirEntries(filenum, &dirHeader);
        }
    }

    return filenum;
}

//    ---------------------------------------------------------
//
//    ResCloseFile() closes an open resource file.
//
//        filenum = file number used when opening file
//    ---------------------------------------------------------

void ResCloseFile(int32_t filenum)
{
    Id id;

    // Make sure file is open
    if (resFile[filenum].fd < 0) {
        Warning("ResCloseFile: filenum %d not in use\n", filenum);
        return;
    }

    // If file being created, flush it
    if (resFile[filenum].pedit) {
        ResWriteDir(filenum);
        ResWriteHeader(filenum);
    }

    // Scan object list, delete any blocks associated with this file
    for (id = ID_MIN; id <= resDescMax; id++) {
        if (ResInUse(id) && (ResFilenum(id) == filenum))
            ResDelete(id);
    }

    // Free up memory
    if (resFile[filenum].pedit) {
        if (resFile[filenum].pedit->pdir)
            free(resFile[filenum].pedit->pdir);
        free(resFile[filenum].pedit);
    }

    // Close file
    close(resFile[filenum].fd);
    resFile[filenum].fd = -1;
}

//    --------------------------------------------------------------
//        INTERNAL ROUTINES
//    ---------------------------------------------------------
//
//    ResFindFreeFilenum() finds free file number

int32_t ResFindFreeFilenum()
{
    int32_t filenum;

    for (filenum = 0; filenum <= MAX_RESFILENUM; filenum++) {
        if (resFile[filenum].fd < 0)
            return filenum;
    }

    return -1;
}

//    ----------------------------------------------------------
//
//    ResReadDirEntries() reads in entries in a directory.
//        (file seek should be set to 1st directory entry)
//
//        filenum  = file number
//        pDirHead = ptr to directory header

void ResReadDirEntries(int32_t filenum, ResDirHeader *pDirHead)
{
#define NUM_DIRENTRY_BLOCK 64        // (12 bytes each)
    int32_t entry,fd;
    int32_t dataOffset;
    ResDirEntry *pDirEntry;
    ResDirEntry dirEntries[NUM_DIRENTRY_BLOCK];

    // Set up
    pDirEntry = &dirEntries[NUM_DIRENTRY_BLOCK]; // no dir entries read
    dataOffset = pDirHead->dataOffset; // mark starting offset
    fd = resFile[filenum].fd;

    // Scan directory:
    for (entry = 0; entry < pDirHead->numEntries; entry++) {
        // If reached end of local directory buffer, refill it
        if (pDirEntry >= &dirEntries[NUM_DIRENTRY_BLOCK]) {
            read(fd, dirEntries, sizeof(ResDirEntry) * NUM_DIRENTRY_BLOCK);
            pDirEntry = &dirEntries[0];
        }

        // Process entry
        ResProcDirEntry(pDirEntry, filenum, dataOffset);

        // Advance file offset and get next
        dataOffset = RES_OFFSET_ALIGN(dataOffset + pDirEntry->csize);
        pDirEntry++;
    }
}

//    -----------------------------------------------------------
//
//    ResProcDirEntry() processes directory entry, sets res desc.
//
//        pDirEntry  = ptr to directory entry
//        filenum     = file number
//        dataOffset = offset in file where data lives

void ResProcDirEntry(ResDirEntry *pDirEntry, int32_t filenum, int32_t dataOffset)
{
    ResDesc *prd;
    int32_t currOffset;

    // Grow table if need to
    ResExtendDesc(pDirEntry->id);

    // If already a resource at this id, warning
    prd = RESDESC(pDirEntry->id);
    if (prd->ptr) {
        Warning("RESOURCE ID COLLISION AT ID %x!!\n",pDirEntry->id);
        ResDelete(pDirEntry->id);
    }

    // Fill in resource descriptor
    prd->ptr = NULL;
    prd->size = pDirEntry->size;
    prd->filenum = filenum;
    prd->lock = 0;
    prd->offset = RES_OFFSET_REAL2DESC(dataOffset);
    prd->flags = pDirEntry->flags;
    prd->type = pDirEntry->type;
    prd->next = 0;
    prd->prev = 0;

    // If loadonopen flag set, load resource
    if (pDirEntry->flags & RDF_LOADONOPEN) {
        currOffset = lseek(resFile[filenum].fd, 0, SEEK_CUR);
        ResLoadResource(pDirEntry->id);
        ResAddToTail(prd);
        lseek(resFile[filenum].fd, currOffset, SEEK_SET);
    }
}

//    --------------------------------------------------------------
//
//    ResReadEditInfo() reads edit info from file.

void ResReadEditInfo(ResFile *prf)
{
    ResEditInfo *pedit = prf->pedit;

    // Init flags to no autopack or anything else
    pedit->flags = 0;

    // Seek to start of file, read in header
    lseek(prf->fd, 0L, SEEK_SET);
    read(prf->fd, &pedit->hdr, sizeof(pedit->hdr));

    // Set no directory (yet, anyway)
    pedit->pdir = NULL;
    pedit->numAllocDir = 0;
    pedit->currDataOffset = 0L;
}

//    ---------------------------------------------------------------
//
//    ResReadDir() reads directory for a file.

void ResReadDir(ResFile *prf, int32_t filenum)
{
    ResEditInfo *pedit;
    ResFileHeader *phead;
    ResDirHeader *pdir;
    ResDirEntry *pDirEntry;
    ResDirHeader dirHead;

    // Read directory header
    pedit = prf->pedit;
    phead = &pedit->hdr;
    lseek(prf->fd, phead->dirOffset, SEEK_SET);
    read(prf->fd, &dirHead, sizeof(ResDirHeader));

    // Allocate space for directory, copy directory header into it
    pedit->numAllocDir = (dirHead.numEntries + DEFAULT_RES_GROWDIRENTRIES) &
            ~(DEFAULT_RES_GROWDIRENTRIES - 1);
    pdir = pedit->pdir = malloc(sizeof(ResDirHeader) + (sizeof(ResDirEntry) * pedit->numAllocDir));
    *pdir = dirHead;

    // Read in directory into allocated space (past header)
    read(prf->fd, RESFILE_DIRENTRY(pdir,0),  dirHead.numEntries * sizeof(ResDirEntry));

    // Scan directory, setting resource descriptors & counting data bytes
    pedit->currDataOffset = pdir->dataOffset;

    RESFILE_FORALLINDIR(pdir, pDirEntry) {
        if (pDirEntry->id == 0)
            pedit->flags |= RFF_NEEDSPACK;
        else
            ResProcDirEntry(pDirEntry, filenum, pedit->currDataOffset);
        pedit->currDataOffset = RES_OFFSET_ALIGN(pedit->currDataOffset + pDirEntry->csize);
    }

    // Seek to current data location
    lseek(prf->fd, pedit->currDataOffset, SEEK_SET);
}

//    --------------------------------------------------------------
//
//    ResCreateEditInfo() creates new empty edit info.

void ResCreateEditInfo(ResFile *prf, int32_t filenum)
{
    ResEditInfo *pedit = prf->pedit;

    pedit->flags = RFF_AUTOPACK;
    memcpy(pedit->hdr.signature, resFileSignature, sizeof(resFileSignature));
    ResSetComment(filenum, "");
    memset(pedit->hdr.reserved, 0, sizeof(pedit->hdr.reserved));
}

//    --------------------------------------------------------------
//
//    ResCreateDir() creates empty dir.

void ResCreateDir(ResFile *prf)
{
    ResEditInfo *pedit = prf->pedit;

    pedit->hdr.dirOffset = 0;
    pedit->numAllocDir = DEFAULT_RES_GROWDIRENTRIES;
    pedit->pdir = malloc(sizeof(ResDirHeader) + (sizeof(ResDirEntry) * pedit->numAllocDir));
    pedit->pdir->numEntries = 0;
    pedit->currDataOffset = pedit->pdir->dataOffset = sizeof(ResFileHeader);
    lseek(prf->fd, pedit->currDataOffset, SEEK_SET);
}

//    -------------------------------------------------------------
//
//    ResWriteDir() writes directory to resource file.

void ResWriteDir(int32_t filenum)
{
    ResFile *prf;

    prf = &resFile[filenum];
    if (prf->pedit == NULL) {
        Warning("ResWriteDir: file %d not open for writing\n", filenum);
        return;
    }

    lseek(prf->fd, prf->pedit->currDataOffset, SEEK_SET);
    write(prf->fd, prf->pedit->pdir, sizeof(ResDirHeader) +
            (prf->pedit->pdir->numEntries * sizeof(ResDirEntry)));
}

//    --------------------------------------------------------
//
//    ResWriteHeader() writes header to resource file.

void ResWriteHeader(int32_t filenum)
{
    ResFile *prf;

    if (resFile[filenum].pedit == NULL) {
        Warning("ResWriteHeader: file %d not open for writing\n", filenum);
        return;
    }

    prf = &resFile[filenum];
    prf->pedit->hdr.dirOffset = prf->pedit->currDataOffset;

    lseek(prf->fd, 0L, SEEK_SET);
    write(prf->fd, &prf->pedit->hdr, sizeof(ResFileHeader));
}
