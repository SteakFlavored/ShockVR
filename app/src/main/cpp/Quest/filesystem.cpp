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

#include "filesystem.h"
#include "shockvr.h"

// The System Shock source code is (almost) all C, so it must be wrapped in
// extern "C" or else the compiler will complain about undefined references.
extern "C" {
#include "res.h"
}

extern "C" {

int32_t LoadResourceFileRO(const char *filename) {
    const std::string resourcePath(shockState.ResourceFolder + filename);

    return ResOpenFile(resourcePath.c_str());
}

int32_t LoadResourceFileRW(const char *filename, const bool create) {
    const std::string resourcePath(shockState.ResourceFolder + filename);

    return ResEditFile(resourcePath.c_str(), create);
}

}
