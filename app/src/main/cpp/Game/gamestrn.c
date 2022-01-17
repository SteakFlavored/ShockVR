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
 * $Source: r:/prj/cit/src/RCS/gamestrn.c $
 * $Revision: 1.17 $
 * $Author: xemu $
 * $Date: 1994/09/14 19:35:47 $
 *
 */

#include <string.h>

#include "Shock.h"
#include "gamestrn.h"
#include "criterr.h"
#include "objsim.h"
#include "objprop.h"
#include "cybstrng.h"

// -------------------
// FILE NAMES AND VARS
// -------------------

#define FILENAME_LEN 						14
#define CONFIG_STRING_RES_FILE		"cfgstrn.res"
#define DEFAULT_STRING_RES_FILE	"cybstrng.rsrc"
#define STRING_RES_VAR					"strings"

// -------
// GLOBALS
// -------

int32_t string_res_file; 							// string res filenum

// ---------
// EXTERNALS
// ---------

//uint8_t *language_files[] = { "\pcybstrng.rsrc", "\pfrnstrng.rsrc", "\pgerstrng.rsrc" } ;
int8_t which_lang;

void init_strings(void)
{
	FSSpec	fSpec;

	// Open the string resource file, Mac style.
	FSMakeFSSpec(gDataVref, gDataDirID, "\pcybstrng.rsrc", &fSpec);
	string_res_file = ResOpenFile(&fSpec);

	if (string_res_file < 0)
		critical_error(CRITERR_RES|0);

	lg_sprintf_install_stringfunc((int8_t*(*)(uint32_t))RefGet);
}

int8_t* get_string(int32_t num, int8_t* buf, int32_t bufsize)
{
   if (!ResInUse(REFID(num)) ||
       !RefIndexValid((RefTable*)ResGet(REFID(num)),REFINDEX(num)))
   {
      if (buf != NULL)
      {
         *buf = '\0';
         return buf;
      }
      else return "";
   }
   if (buf != NULL)
   {
      int8_t* s = (int8_t *)RefLock(num);
      if (s != NULL)
      {
         strncpy(buf,s,bufsize);
         buf[bufsize-1] = '\0';
      }
      RefUnlock(num);
      return ( s == NULL) ? NULL : buf;
   }
   else return get_temp_string(num);
}

int8_t* get_temp_string(int32_t num)
{
   return (int8_t*)RefGet(num);
}

int8_t* get_object_short_name(int32_t trip, int8_t* buf, int32_t bufsize)
{
   return get_string(MKREF(RES_objshortnames,OPTRIP(trip)),buf,bufsize);
}

int8_t* get_object_long_name(int32_t trip, int8_t* buf, int32_t bufsize)
{
   return get_string(MKREF(RES_objlongnames,OPTRIP(trip)),buf,bufsize);
}

void shutdown_strings(void)
{
   ResCloseFile(string_res_file);
}

int8_t* get_texture_name(int32_t abs_texture,int8_t* buf, int32_t bufsiz)
{
   return get_string(MKREF(RES_texnames,abs_texture),buf,bufsiz);
}


int8_t* get_texture_use_string(int32_t abs_texture, int8_t* buf, int32_t bufsiz)
{
   return get_string(MKREF(RES_texuse,abs_texture), buf, bufsiz);
}
