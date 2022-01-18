/*

Core event loop for the ShockVR project
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

#include "shockvr.h"

// The System Shock source code is (almost) all C, so it must be wrapped in
// extern "C" or else the compiler will complain about undefined references.
extern "C" {
#include "res.h"
}

struct AndroidAppState {
    bool Resumed = false;
};

/**
 * Process the next main command.
 */
static void app_handle_cmd(struct android_app* app, int32_t cmd) {
    AndroidAppState* appState = (AndroidAppState*)app->userData;

    switch (cmd) {
        // There is no APP_CMD_CREATE. The ANativeActivity creates the
        // application thread from onCreate(). The application thread
        // then calls android_main().
        case APP_CMD_START: {
            LOGI("    APP_CMD_START");
            LOGI("onStart()");
            break;
        }
        case APP_CMD_RESUME: {
            LOGI("onResume()");
            LOGI("    APP_CMD_RESUME");
            appState->Resumed = true;
            break;
        }
        case APP_CMD_PAUSE: {
            LOGI("onPause()");
            LOGI("    APP_CMD_PAUSE");
            appState->Resumed = false;
            break;
        }
        case APP_CMD_STOP: {
            LOGI("onStop()");
            LOGI("    APP_CMD_STOP");
            break;
        }
        case APP_CMD_DESTROY: {
            LOGI("onDestroy()");
            LOGI("    APP_CMD_DESTROY");
            break;
        }
        case APP_CMD_INIT_WINDOW: {
            LOGI("surfaceCreated()");
            LOGI("    APP_CMD_INIT_WINDOW");
            break;
        }
        case APP_CMD_TERM_WINDOW: {
            LOGI("surfaceDestroyed()");
            LOGI("    APP_CMD_TERM_WINDOW");
            break;
        }
    }
}

int ShockVrMain(struct android_app* app) {
    AndroidAppState appState = {};

    app->userData = &appState;
    app->onAppCmd = app_handle_cmd;

    // Initialize the resource manager
    ResInit();

    while (app->destroyRequested == 0) {
        // Read all pending events.
        for (;;) {
            int32_t events;
            struct android_poll_source* source;
            // If the timeout is zero, returns immediately without blocking.
            // If the timeout is negative, waits indefinitely until an event appears.
            const int32_t timeoutMilliseconds =
                    (!appState.Resumed && app->destroyRequested == 0) ? -1 : 0;
            if (ALooper_pollAll(timeoutMilliseconds, nullptr, &events, (void**)&source) < 0) {
                break;
            }

            // Process this event.
            if (source != nullptr) {
                source->process(app, source);
            }
        }
    }

    return 0;
}
