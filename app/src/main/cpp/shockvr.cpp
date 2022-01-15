#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ShockVR", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "ShockVR", __VA_ARGS__))

struct AndroidAppState {
    bool Resumed = false;
};

extern "C" {
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

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* app) {
    JNIEnv* Env;
    app->activity->vm->AttachCurrentThread(&Env, nullptr);

    AndroidAppState appState = {};

    app->userData = &appState;
    app->onAppCmd = app_handle_cmd;

    while (app->destroyRequested == 0) {
        // Read all pending events.
        for (;;) {
            int events;
            struct android_poll_source* source;
            // If the timeout is zero, returns immediately without blocking.
            // If the timeout is negative, waits indefinitely until an event appears.
            const int timeoutMilliseconds =
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

    app->activity->vm->DetachCurrentThread();
}
}