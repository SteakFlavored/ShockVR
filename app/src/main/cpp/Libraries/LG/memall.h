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
//        Memall.H        Memory allocator
//        Rex E. Bradford (REX)
/*
* $Header: n:/project/lib/src/lg/rcs/memall.h 1.12 1993/12/20 13:40:05 ept Exp $
* $Log: memall.h $
 * Revision 1.12  1993/12/20  13:40:05  ept
 * Added MemStackRealloc.
 *
 * Revision 1.11  1993/09/30  18:36:51  rex
 * Added prototypes for memgrow.c stuff (heap grow, lock, unlock)
 *
 * Revision 1.10  1993/09/13  12:40:12  dfan
 * ptr and size were reserved words in assembler
 *
 * Revision 1.9  1993/09/13  11:11:13  dfan
 * Add memstack stuff
 *
 * Revision 1.8  1993/08/11  18:44:12  rex
 * Changed Calloc() to macro, so can do spew
 *
 * Revision 1.7  1993/08/11  17:30:40  rex
 * Added Spew() versions of malloc/realloc/free, and made uppercase macro
 * versions point to them when DBG is on
 *
 * Revision 1.6  1993/04/16  12:59:55  matt
 * Added some void's to functions that took no args to get around h2inc bug.
 *
 * Revision 1.5  1993/04/13  16:05:22  rex
 * Added prototypes for MemCheckOn() and MemCheckOff()
 *
 * Revision 1.4  1993/03/24  12:02:44  matt
 * More asm header junk
 *
 * Revision 1.3  1993/03/16  15:12:39  matt
 * Added junk for H2INC translation to assembly header
 *
 * Revision 1.2  1993/02/05  17:38:07  rex
 * Added Calloc() prototype
 *
 * Revision 1.1  1993/01/29  09:47:55  rex
 * Initial revision
 *
 * Revision 1.3  1993/01/18  12:18:38  rex
 * Changed interface to new standard we all agreed on
 *
 * Revision 1.2  1993/01/14  09:41:02  rex
 * Decided Malloc() should never fail, changed interface.
 *
 * Revision 1.1  1993/01/12  17:54:54  rex
 * Initial revision
 *
*/


#ifndef MEMALL_H
#define MEMALL_H


//#include <malloc.h>
#include "lg_types.h"

/*
//    Setting, pushing, & popping allocator sets

void MemSetAllocator(void *(*fm)(size_t size),
    void *(*fr)(void *p, size_t size), void (*ff)(void *p));
int32_t MemPushAllocator(void *(*fm)(size_t size),
    void *(*fr)(void *p, size_t size), void (*ff)());
int32_t MemPopAllocator(void);

//    Allocating, reallocating, & freeing memory

extern void *(*f_malloc)(size_t size);
extern void *(*f_realloc)(void *p, size_t size);
extern void (*f_free)(void *p);

#ifdef DBG_ON

void *MallocSpew(size_t size, int8_t *file, int32_t line);
void *ReallocSpew(void *p, size_t size, int8_t *file, int32_t line);
void FreeSpew(void *p, int8_t *file, int32_t line);
void *CallocSpew(size_t size, int8_t *file, int32_t line);

#define Malloc(size) MallocSpew(size,__FILE__,__LINE__)
#define Realloc(p,size) ReallocSpew(p,size,__FILE__,__LINE__)
#define Free(p) FreeSpew(p,__FILE__,__LINE__)
#define Calloc(size) CallocSpew(size,__FILE__,__LINE__)

#else

void *CallocNorm(size_t size);

#define Malloc(size) (*f_malloc)(size)
#define Realloc(p,size) (*f_realloc)(p,size)
#define Free(p) (*f_free)(p)
#define Calloc(size) CallocNorm(size)

#endif

#ifdef _H2INC                    //if translating, include assembly macros
#include "memmacro.h"        //this will translate to 'include memmacro.inc'
#endif

//    Memory checking

void MemCheckOn(bool hard);
void MemCheckOff(void);

//    Heap management (memgrow.c)

int32_t MemGrowHeap(int32_t wantK);
void MemLockHeap();
void MemUnlockHeap();

*/

//////////////////////////////
//
// Dealing with a large block of memory as a stack for easy allocation
//
// Rationale: often, routines want some large amount of memory, and know they
// will throw it away when they're done.  If you use a MemStack for this memory,
// you don't have to worry about fragmenting the heap.
//
// To use, declare a MemStack.  Malloc n bytes of memory, put the resulting
// pointer in baseptr and n in size, and call MemStackInit().  Then use
// MemStackAlloc() and MemStackFree() to grab and release memory.  You must
// free memory in the reverse order of allocating it, as this is a stack.
// Note that you can have multiple MemStacks if you feel like it.

typedef struct
{
    void *baseptr; // pointer to bottom of stack
    int32_t  sz;        // size of stack in bytes
    void *topptr;  // pointer to current top of stack (next free byte)
}
MemStack;

void MemStackInit (MemStack *ms);
void *MemStackAlloc (MemStack *ms, int32_t size);
void *MemStackRealloc (MemStack *ms, void *ptr, int32_t newsize);
bool MemStackFree (MemStack *ms, void *ptr);


#endif
