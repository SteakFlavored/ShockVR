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
#include "lg.h"
#include "error.h"
#include "mouse.h"
#include "vmouse.h"
//#include <mprintf.h>

void (*ui_mouse_convert)(int16_t *px, int16_t *py, bool down) = NULL;
void (*ui_mouse_convert_round)(int16_t *px, int16_t *py, bool down) = NULL;

errtype ui_mouse_do_conversion(int16_t *pmx, int16_t *pmy, bool down)
{
    if (ui_mouse_convert != NULL)
        ui_mouse_convert(pmx,pmy,down);
    return(OK);
}

errtype ui_mouse_get_xy(int16_t *pmx, int16_t *pmy)
{
    errtype retval;
    retval = mouse_get_xy(pmx,pmy);
    ui_mouse_do_conversion(pmx,pmy,true);
    return(retval);
}

errtype ui_mouse_put_xy(int16_t pmx, int16_t pmy)
{
    errtype retval;
    ui_mouse_do_conversion(&pmx,&pmy,false);
    retval = mouse_put_xy(pmx,pmy);
    return(retval);
}

errtype ui_mouse_constrain_xy(int16_t xl, int16_t yl, int16_t xh, int16_t yh)
{
    if (ui_mouse_convert == NULL)
        return(mouse_constrain_xy(xl,yl,xh,yh));
    else
    {
        int16_t uxl,uyl,uxh,uyh;
        uxl=xl;
        uyl=yl;
        uxh=xh;
        uyh=yh;
        ui_mouse_convert_round(&uxl,&uyl,false);
        ui_mouse_convert_round(&uxh,&uyh,false);
        return(mouse_constrain_xy(uxl,uyl,uxh,uyh));
    }
}
