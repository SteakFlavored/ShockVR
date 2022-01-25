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
#include "filesystem.h"
#include "framebuffer.h"

#include <openxr/openxr_oculus_helpers.h>

// The System Shock source code is (almost) all C, so it must be wrapped in
// extern "C" or else the compiler will complain about undefined references.
extern "C" {
#include "res.h"
}

// Big gross shared state, but this makes it easier to deal with C/C++ interop.
extern "C" {
ShockState shockState;
}

/**
 * Process the next main command.
 */
static void app_handle_cmd(struct android_app* app, int32_t cmd) {
    ShockState* appState = (ShockState*)app->userData;

    switch (cmd) {
        // There is no APP_CMD_CREATE. The ANativeActivity creates the
        // application thread from onCreate(). The application thread
        // then calls android_main().
        case APP_CMD_START: {
            break;
        }
        case APP_CMD_RESUME: {
            appState->Resumed = true;
            break;
        }
        case APP_CMD_PAUSE: {
            appState->Resumed = false;
            break;
        }
        case APP_CMD_STOP: {
            break;
        }
        case APP_CMD_DESTROY: {
            break;
        }
        case APP_CMD_INIT_WINDOW: {
            break;
        }
        case APP_CMD_TERM_WINDOW: {
            break;
        }
    }
}

int ShockVrMain(struct android_app* app) {
    app->userData = &shockState;
    app->onAppCmd = app_handle_cmd;

    int ret = InitOpenXR(app);
    if (ret != 0) {
        // Bail out if we failed to set up OpenXR.
        return ret;
    }

    // Set the global pointing to the resource folder for filesystem ops.
    shockState.ResourceFolder = app->activity->externalDataPath;
    shockState.ResourceFolder += "/res/";

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
                    (!shockState.Resumed && app->destroyRequested == 0) ? -1 : 0;
            if (ALooper_pollAll(timeoutMilliseconds, nullptr, &events, (void**)&source) < 0) {
                break;
            }

            // Process this event.
            if (source != nullptr) {
                source->process(app, source);
            }
        }

        HandleXrEvents();
        if (!shockState.SessionActive) {
            continue;
        }

        XrFrameWaitInfo waitFrameInfo = {};
        waitFrameInfo.type = XR_TYPE_FRAME_WAIT_INFO;
        waitFrameInfo.next = NULL;

        XrFrameState frameState = {};
        frameState.type = XR_TYPE_FRAME_STATE;
        frameState.next = NULL;

        xrWaitFrame(shockState.Session, &waitFrameInfo, &frameState);

        XrFrameBeginInfo beginFrameDesc = {};
        beginFrameDesc.type = XR_TYPE_FRAME_BEGIN_INFO;
        beginFrameDesc.next = NULL;
        xrBeginFrame(shockState.Session, &beginFrameDesc);

        XrSpaceLocation loc = {};
        loc.type = XR_TYPE_SPACE_LOCATION;
        xrLocateSpace(shockState.HeadSpace, shockState.LocalSpace, frameState.predictedDisplayTime, &loc);

        XrFrameEndInfo endFrameInfo = {};
        endFrameInfo.type = XR_TYPE_FRAME_END_INFO;
        endFrameInfo.displayTime = frameState.predictedDisplayTime;
        endFrameInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
        endFrameInfo.layerCount = 0;
        endFrameInfo.layers = nullptr;
        xrEndFrame(shockState.Session, &endFrameInfo);
    }

    ShutdownOpenXR();

    return 0;
}
