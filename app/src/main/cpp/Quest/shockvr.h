/*

Core header for the ShockVR project
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

#include <android/log.h>
#include <android_native_app_glue.h>

#include <string>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ShockVR", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "ShockVR", __VA_ARGS__))

// Big gross shared state, but this makes it easier to deal with C/C++ interop.
extern "C" {
struct ShockState {
    std::string ResourceFolder;
    bool Resumed = false;
};

extern ShockState shockState;
}

int ShockVrMain(struct android_app *app);
