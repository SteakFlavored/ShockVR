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

#include "cybstrng.h"
#include "filesystem.h"
#include "gamestrn.h"
#include "lgsprntf.h"
#include "res.h"

// -------
// GLOBALS
// -------

int32_t string_res_file;                             // string res filenum

// ---------
// EXTERNALS
// ---------

const char *language_files[] = { "cybstrng.res", "frnstrng.res", "gerstrng.res" };
int8_t which_lang = 0;

void init_strings(void)
{
    // If 'which_lang' is out-of-range for some reason fall back on English.
    if ((uint8_t)which_lang > sizeof(language_files) / sizeof(*language_files))
        which_lang = 0;

    const char *language_filename = language_files[which_lang];
    string_res_file = LoadResourceFileRO(language_filename);
    if (string_res_file < 0) {
        // LOGE("Could not open string resource '%s': %d", language_filename, string_res_file);
        // critical_error(CRITERR_RES|0);
        exit(1);
    }

    lg_sprintf_install_stringfunc((char*(*)(uint32_t))RefGet);
}

char* get_string(int32_t num, char* buf, int32_t bufsize)
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
        char* s = (char *)RefLock(num);
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

char* get_temp_string(int32_t num)
{
    return (char*)RefGet(num);
}

char* get_object_short_name(int32_t trip, char* buf, int32_t bufsize)
{
    return NULL; //get_string(MKREF(RES_objshortnames,OPTRIP(trip)),buf,bufsize);
}

char* get_object_long_name(int32_t trip, char* buf, int32_t bufsize)
{
    return NULL; // get_string(MKREF(RES_objlongnames,OPTRIP(trip)),buf,bufsize);
}

void shutdown_strings(void)
{
    ResCloseFile(string_res_file);
}

char* get_texture_name(int32_t abs_texture, char* buf, int32_t bufsiz)
{
    return get_string(MKREF(RES_texnames,abs_texture),buf,bufsiz);
}


char* get_texture_use_string(int32_t abs_texture, char* buf, int32_t bufsiz)
{
    return get_string(MKREF(RES_texuse,abs_texture), buf, bufsiz);
}
