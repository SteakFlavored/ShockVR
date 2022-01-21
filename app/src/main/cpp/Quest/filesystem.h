/*

Filesystem abstraction for the ShockVR project
Copyright 2022 Matt Fulghum <mfulghum@gmail.com>

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

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t LoadResourceFileRO(const char *filename);
int32_t LoadResourceFileRW(const char *filename, const bool create);

#ifdef __cplusplus
} // extern "C"
#endif
