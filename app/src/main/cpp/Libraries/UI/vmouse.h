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
// virtualized mouse support
extern errtype ui_mouse_get_xy(int16_t *pmx, int16_t *pmy);
extern errtype ui_mouse_put_xy(int16_t pmx, int16_t pmy);
extern errtype ui_mouse_constrain_xy(int16_t xl, int16_t yl, int16_t xh, int16_t yh);
extern errtype ui_mouse_do_conversion(int16_t *pmx, int16_t *pmy, bool down);
