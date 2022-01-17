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
 * $Source: r:/prj/lib/src/lg/rcs/tmpalloc.h $
 * $Revision: 1.4 $
 * $Author: kaboom $
 * $Date: 1994/08/03 23:09:27 $
 *
 * Header for routines for controlling temporary stacks of big_buffer
 *
 * This file is part of the 2d library.
 */

extern MemStack *temp_mem_get_stack(void);
extern int32_t temp_mem_init(MemStack *ms);
extern int32_t temp_mem_uninit(void);
extern void *temp_malloc(int32_t n);
extern void *temp_realloc(void *p,int32_t n);
extern int32_t temp_free(void *p);

#ifdef DBG_ON
extern int32_t temp_spew_mem_init(MemStack *ms,int8_t *file,int32_t line);
extern int32_t temp_spew_mem_uninit(int8_t *file,int32_t line);
extern void *temp_spew_malloc(int32_t n,int8_t *file,int32_t line);
extern void *temp_spew_realloc(void *p,int32_t n,int8_t *file,int32_t line);
extern int32_t temp_spew_free(void *p,int8_t *file,int32_t line);

#define TempMemInit(ms) temp_spew_mem_init(ms,__FILE__,__LINE__)
#define TempMemUninit() temp_spew_mem_uninit(__FILE__,__LINE__)
#define TempMalloc(n) temp_spew_malloc(n,__FILE__,__LINE__)
#define TempRealloc(p,n) temp_spew_realloc(p,n,__FILE__,__LINE__)
#define TempFree(p) temp_spew_free(p,__FILE__,__LINE__)
#else /* !DBG_ON */
#define TempMemInit temp_mem_init
#define TempMemUninit temp_mem_uninit
#define TempMalloc temp_malloc
#define TempRealloc temp_realloc
#define TempFree temp_free
#endif /* DBG_ON */
