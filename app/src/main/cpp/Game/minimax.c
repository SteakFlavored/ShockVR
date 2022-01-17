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
 * $Source: r:/prj/cit/src/RCS/minimax.c $
 * $Revision: 1.2 $
 * $Author: tjs $
 * $Date: 1994/09/28 17:44:13 $
 *
 */

#ifdef LOST_TREASURES_OF_MFD_GAMES
#include <limits.h>
#include <string.h>
#include "minimax.h"

#define MAX_POSITION_SIZE 32

static uint8_t* FauxStackBase;
static uint8_t* FauxStackPointer;
static uint32_t FauxStackSize;

static uint32_t PositionSize;
static bool (*generate_position)(void* parent, int32_t index, bool minimizer_moves);
static int32_t (*static_evaluator)(void* position);
static bool (*extend_horizon)(void* position);

#define StackSpew(x)

void fstack_push(void* data, uint32_t siz);
void fstack_pop(void* data, uint32_t siz);
void* fstack_create(uint32_t siz);
void fstack_flush(uint32_t siz);


void fstack_init(uint8_t* fs, uint32_t siz)
{
   FauxStackSize=siz;
   FauxStackBase=FauxStackPointer=fs;
}

void fstack_push(void* data, uint32_t siz)
{
   memcpy(FauxStackPointer,data,siz);
   FauxStackPointer+=siz;
   StackSpew(("pushing %d bytes, top %d\n",siz,FauxStackPointer-FauxStackBase));
}

void fstack_pop(void* data, uint32_t siz)
{
   FauxStackPointer-=siz;
   memcpy(data,FauxStackPointer,siz);
   StackSpew(("popping %d bytes, top %d\n",siz,FauxStackPointer-FauxStackBase));
}

void* fstack_create(uint32_t siz)
{
   void* retval;
   retval=FauxStackPointer;
   FauxStackPointer+=siz;
   StackSpew(("moving top %d bytes, top %d\n",siz,FauxStackPointer-FauxStackBase));
   return((void*)retval);
}

void fstack_flush(uint32_t siz)
{
   FauxStackPointer-=siz;
   StackSpew(("flushing top %d bytes, top %d\n",siz,FauxStackPointer-FauxStackBase));
}

#define FSTACK_PUSHVAR(v) fstack_push(&v,sizeof(v))
#define FSTACK_POPVAR(v) fstack_pop(&v,sizeof(v))

// ------------
// MiniMax procedure
//   Runs MiniMax evaluation of board position, using user-supplied
// stack for recursion.
//

void minimax_get_result(int32_t* val,int8_t* which)
{
   fstack_pop(val,sizeof(int32_t));
   fstack_pop(which,sizeof(int8_t));
}

bool minimax_done(void)
{
   // done if only value (an int) and move (a char) remain on stack
   return(FauxStackPointer<=FauxStackBase+(sizeof(int32_t)+sizeof(int8_t)));
}

void minimax_setup(void* boardpos, uint32_t pos_siz, int8_t depth, bool minimize,
   int32_t (*evaluator)(void*), bool (*generate)(void*,int32_t,bool),
   bool (*horizon)(void*))
{
   int8_t next_child=0;

   static_evaluator=evaluator;
   generate_position=generate;
   extend_horizon=horizon;
   PositionSize=pos_siz;

   // correspondence of these stack operations to the variables
   // used in minimax_step commented below, even the obvious ones.
   //
   FSTACK_PUSHVAR(next_child);      // next_child
   fstack_create(sizeof(int32_t));      // bestval
   fstack_create(sizeof(int8_t));     // which_child
   FSTACK_PUSHVAR(minimize);        // minimize
   FSTACK_PUSHVAR(depth);           // depth
   fstack_push(boardpos,pos_siz);   // boardpos
   fstack_create(sizeof(int8_t)+sizeof(int32_t));    // return values
}

void minimax_step(void)
{
   uint8_t boardpos[MAX_POSITION_SIZE];
   uint8_t copy[MAX_POSITION_SIZE];
   int32_t value, bestval;
   int8_t next_child, which_child;
   uint8_t depth;
   bool minimize;

   FSTACK_POPVAR(value);  // get value from previous recursive call
   fstack_flush(sizeof(which_child));  // flush which child gave best value

   fstack_pop(boardpos, PositionSize);
   FSTACK_POPVAR(depth);
   FSTACK_POPVAR(minimize);
   FSTACK_POPVAR(which_child);
   FSTACK_POPVAR(bestval);
   FSTACK_POPVAR(next_child);

   if(depth==0) {
      if(extend_horizon && extend_horizon(boardpos)) {
         depth=1;
      }
      else {
         value=static_evaluator(boardpos);
         fstack_create(sizeof(which_child));
         FSTACK_PUSHVAR(value);
         return;
      }
   }

   if(next_child<=0) {
      bestval=minimize?INT_MAX:INT_MIN;
   }
   else {
      if((!minimize) ^ (value<bestval)) {  // new value is better than previous
         bestval=value;
         which_child=next_child-1;
      }
   }

#define PRUNE_WIN
#ifdef PRUNE_WIN
   // if have already found a winning move, do not continue
   if(bestval==(minimize?INT_MIN:INT_MAX)) {
      FSTACK_PUSHVAR(which_child);
      FSTACK_PUSHVAR(bestval);
      return;
   }
#endif

   memcpy(copy,boardpos,PositionSize);
   if(!generate_position(copy,next_child,minimize)) {  // no more children to be had
      if(next_child==0) // there were no children at all!
         bestval=static_evaluator(boardpos);
      FSTACK_PUSHVAR(which_child);
      FSTACK_PUSHVAR(bestval);
      return;
   }

   // before setting up for recursion, preserve our local parameters
   next_child++;
   FSTACK_PUSHVAR(next_child);
   FSTACK_PUSHVAR(bestval);
   FSTACK_PUSHVAR(which_child);
   fstack_create(sizeof(minimize));
   FSTACK_PUSHVAR(depth);
   fstack_create(PositionSize);

   // set up for recursive call
   next_child=0; FSTACK_PUSHVAR(next_child);
   FSTACK_PUSHVAR(bestval);
   FSTACK_PUSHVAR(which_child);
   minimize=!minimize; FSTACK_PUSHVAR(minimize);
   depth--; FSTACK_PUSHVAR(depth);
   fstack_push(copy,PositionSize);

   // the actual value we push here is never looked at, but we need to
   // make space for it.
   fstack_create(sizeof(which_child)+sizeof(value));
}

#endif // LOST_TREASURES...
