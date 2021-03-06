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
//    This seems silly now, but later it will all make sense, sensei...
//    =================================================================
//int    EDMS_FF_cast_projectile( Q *X, Q D[3], Q speed, Q mass, Q size, Q range, int32_t exclude, physics_handle shooter );
physics_handle EDMS_FF_cast_projectile(    Q *X,
                                                        Q D[3],
                                                        Q speed,
                                                        Q mass,
                                                        Q size,
                                                        Q range,
                                                        int32_t exclude,
                                                        int32_t shooter,
                                                        int32_t &g_info,
                                                        int32_t &w_info,
                                                        bool &hit );

physics_handle EDMS_FF_cast_projectile_new(
                                                        Q *X,
                                                        Q D[3],
                                                        Q speed,
                                                        Q mass,
                                                        Q size,
                                                        Q range,
                                                        int32_t exclude,
                                                        int32_t shooter,
                                                        int32_t &g_info,
                                                        int32_t &w_info,
                                                        bool &hit );
