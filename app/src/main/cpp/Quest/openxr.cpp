/*

OpenXR setup code for the ShockVR project
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

int InitOpenXR(android_app *app) {
    XrResult xrResult;

    shockState.SessionActive = false;

    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR;
    xrResult = xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR",
            (PFN_xrVoidFunction*)&xrInitializeLoaderKHR);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get xrInitializeLoader function pointer.");
        return 1;
    }

    XrLoaderInitInfoAndroidKHR loaderInitializeInfoAndroid;
    memset(&loaderInitializeInfoAndroid, 0, sizeof(loaderInitializeInfoAndroid));
    loaderInitializeInfoAndroid.type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR;
    loaderInitializeInfoAndroid.next = NULL;
    loaderInitializeInfoAndroid.applicationVM = app->activity->vm;
    loaderInitializeInfoAndroid.applicationContext = app->activity->clazz;
    xrInitializeLoaderKHR((XrLoaderInitInfoBaseHeaderKHR*)&loaderInitializeInfoAndroid);

    const char* const requiredExtensionNames[] = {
            XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME,
            XR_KHR_ANDROID_SURFACE_SWAPCHAIN_EXTENSION_NAME};
    const uint32_t numRequiredExtensions = sizeof(requiredExtensionNames) / sizeof(requiredExtensionNames[0]);

    // Check the list of required extensions against what is supported by the runtime.
    {
        PFN_xrEnumerateInstanceExtensionProperties xrEnumerateInstanceExtensionProperties;
        xrResult = xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrEnumerateInstanceExtensionProperties",
                (PFN_xrVoidFunction*)&xrEnumerateInstanceExtensionProperties);
        if (xrResult != XR_SUCCESS) {
            LOGE("Failed to get xrEnumerateInstanceExtensionProperties function pointer.");
            return 1;
        }

        uint32_t numInputExtensions = 0;
        uint32_t numOutputExtensions = 0;
        xrEnumerateInstanceExtensionProperties(nullptr, numInputExtensions, &numOutputExtensions, nullptr);

        numInputExtensions = numOutputExtensions;

        XrExtensionProperties* extensionProperties = new XrExtensionProperties[numOutputExtensions];

        for (uint32_t i = 0; i < numOutputExtensions; i++) {
            extensionProperties[i].type = XR_TYPE_EXTENSION_PROPERTIES;
            extensionProperties[i].next = nullptr;
        }

        xrEnumerateInstanceExtensionProperties(nullptr, numInputExtensions, &numOutputExtensions, extensionProperties);
        for (uint32_t i = 0; i < numRequiredExtensions; i++) {
            bool found = false;
            for (uint32_t j = 0; j < numOutputExtensions; j++) {
                if (!strcmp(requiredExtensionNames[i], extensionProperties[j].extensionName)) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                LOGE("Failed to find required extension %s", requiredExtensionNames[i]);
                return 1;
            }
        }

        delete[] extensionProperties;
    }

    // Create the OpenXR instance.
    XrApplicationInfo appInfo;
    memset(&appInfo, 0, sizeof(appInfo));
    strcpy(appInfo.applicationName, "ShockVR");
    appInfo.applicationVersion = 0;
    strcpy(appInfo.engineName, "System Shock VR");
    appInfo.engineVersion = 0;
    appInfo.apiVersion = XR_CURRENT_API_VERSION;

    XrInstanceCreateInfo instanceCreateInfo;
    memset(&instanceCreateInfo, 0, sizeof(instanceCreateInfo));
    instanceCreateInfo.type = XR_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.next = NULL;
    instanceCreateInfo.createFlags = 0;
    instanceCreateInfo.applicationInfo = appInfo;
    instanceCreateInfo.enabledApiLayerCount = 0;
    instanceCreateInfo.enabledApiLayerNames = NULL;
    instanceCreateInfo.enabledExtensionCount = numRequiredExtensions;
    instanceCreateInfo.enabledExtensionNames = requiredExtensionNames;

    xrResult = xrCreateInstance(&instanceCreateInfo, &shockState.Instance);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to create XR instance: %d.", xrResult);
        return 1;
    }

    XrSystemGetInfo systemGetInfo;
    memset(&systemGetInfo, 0, sizeof(systemGetInfo));
    systemGetInfo.type = XR_TYPE_SYSTEM_GET_INFO;
    systemGetInfo.next = NULL;
    systemGetInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    xrResult = xrGetSystem(shockState.Instance, &systemGetInfo, &shockState.SystemId);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get system.");
        return 1;
    }

    // Get the graphics requirements.
    PFN_xrGetOpenGLESGraphicsRequirementsKHR pfnGetOpenGLESGraphicsRequirementsKHR = NULL;
    xrGetInstanceProcAddr(shockState.Instance, "xrGetOpenGLESGraphicsRequirementsKHR",
        (PFN_xrVoidFunction*)(&pfnGetOpenGLESGraphicsRequirementsKHR));

    XrGraphicsRequirementsOpenGLESKHR graphicsRequirements = {};
    graphicsRequirements.type = XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR;
    pfnGetOpenGLESGraphicsRequirementsKHR(shockState.Instance, shockState.SystemId, &graphicsRequirements);

    // Configure EGL
    shockState.EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    EGLint majorVersion;
    EGLint minorVersion;
    eglInitialize(shockState.EglDisplay, &majorVersion, &minorVersion);

    const int MAX_CONFIGS = 1024;
    EGLConfig configs[MAX_CONFIGS];
    EGLint numConfigs = 0;
    if (eglGetConfigs(shockState.EglDisplay, configs, MAX_CONFIGS, &numConfigs) == EGL_FALSE) {
        LOGE("eglGetConfigs() failed: %d", eglGetError());
        return 1;
    }

    const EGLint configAttribs[] = {
        EGL_RED_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_BLUE_SIZE,
        8,
        EGL_ALPHA_SIZE,
        8,
        EGL_DEPTH_SIZE,
        0,
        EGL_STENCIL_SIZE,
        0,
        EGL_SAMPLES,
        0,
        EGL_NONE};

    shockState.EglConfig = 0;
    for (int i = 0; i < numConfigs; i++) {
        EGLint value = 0;

        eglGetConfigAttrib(shockState.EglDisplay, configs[i], EGL_RENDERABLE_TYPE, &value);
        if ((value & EGL_OPENGL_ES3_BIT_KHR) != EGL_OPENGL_ES3_BIT_KHR) {
            continue;
        }

        // The pbuffer config also needs to be compatible with normal window rendering
        // so it can share textures with the window context.
        eglGetConfigAttrib(shockState.EglDisplay, configs[i], EGL_SURFACE_TYPE, &value);
        if ((value & (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) != (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) {
            continue;
        }

        int j = 0;
        for (; configAttribs[j] != EGL_NONE; j += 2) {
            eglGetConfigAttrib(shockState.EglDisplay, configs[i], configAttribs[j], &value);
            if (value != configAttribs[j + 1]) {
                break;
            }
        }
        if (configAttribs[j] == EGL_NONE) {
            shockState.EglConfig = configs[i];
            break;
        }
    }
    if (shockState.EglConfig == 0) {
        LOGE("eglChooseConfig() failed: %d", eglGetError());
        return 1;
    }

    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    shockState.EglContext = eglCreateContext(shockState.EglDisplay, shockState.EglConfig, EGL_NO_CONTEXT, contextAttribs);
    if (shockState.EglContext == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext() failed: %d", eglGetError());
        return 1;
    }

    if (eglMakeCurrent(shockState.EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, shockState.EglContext) == EGL_FALSE) {
        LOGE("eglMakeCurrent() failed: %d", eglGetError());
        eglDestroyContext(shockState.EglDisplay, shockState.EglContext);
        shockState.EglContext = EGL_NO_CONTEXT;
        return 1;
    }

    // Create the OpenXR Session.
    XrGraphicsBindingOpenGLESAndroidKHR graphicsBindingAndroidGLES = {};
    graphicsBindingAndroidGLES.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR;
    graphicsBindingAndroidGLES.next = NULL;
    graphicsBindingAndroidGLES.display = shockState.EglDisplay;
    graphicsBindingAndroidGLES.config = shockState.EglConfig;
    graphicsBindingAndroidGLES.context = shockState.EglContext;

    XrSessionCreateInfo sessionCreateInfo = {};
    memset(&sessionCreateInfo, 0, sizeof(sessionCreateInfo));
    sessionCreateInfo.type = XR_TYPE_SESSION_CREATE_INFO;
    sessionCreateInfo.next = &graphicsBindingAndroidGLES;
    sessionCreateInfo.createFlags = 0;
    sessionCreateInfo.systemId = shockState.SystemId;

    xrResult = xrCreateSession(shockState.Instance, &sessionCreateInfo, &shockState.Session);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to create XR session: %d.", xrResult);
        return 1;
    }

    // App only supports the primary stereo view config.
    const XrViewConfigurationType supportedViewConfigType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

    // Enumerate the viewport configurations.
    uint32_t viewportConfigTypeCount = 0;
    xrEnumerateViewConfigurations(shockState.Instance, shockState.SystemId, 0, &viewportConfigTypeCount, nullptr);

    XrViewConfigurationType* viewportConfigurationTypes = new XrViewConfigurationType[viewportConfigTypeCount];
    xrEnumerateViewConfigurations(shockState.Instance, shockState.SystemId, viewportConfigTypeCount,
            &viewportConfigTypeCount, viewportConfigurationTypes);

    for (uint32_t i = 0; i < viewportConfigTypeCount; i++) {
        const XrViewConfigurationType viewportConfigType = viewportConfigurationTypes[i];

        XrViewConfigurationProperties viewportConfig;
        viewportConfig.type = XR_TYPE_VIEW_CONFIGURATION_PROPERTIES;
        xrGetViewConfigurationProperties(shockState.Instance, shockState.SystemId, viewportConfigType, &viewportConfig);

        uint32_t viewCount;
        xrEnumerateViewConfigurationViews(shockState.Instance, shockState.SystemId, viewportConfigType, 0, &viewCount,
                nullptr);

        if (viewCount > 0) {
            XrViewConfigurationView* elements = new XrViewConfigurationView[viewCount];
            for (uint32_t eye = 0; eye < viewCount; eye++) {
                elements[eye].type = XR_TYPE_VIEW_CONFIGURATION_VIEW;
                elements[eye].next = nullptr;
            }

            // Cache the view config properties for the selected config type.
            xrEnumerateViewConfigurationViews(shockState.Instance, shockState.SystemId, viewportConfigType, viewCount,
                    &viewCount, elements);
            if (viewportConfigType == supportedViewConfigType) {
                for (uint32_t eye = 0; eye < viewCount; eye++) {
                    shockState.ViewConfigurationView[eye] = elements[eye];
                }
            }
            delete[] elements;
        }
    }
    delete[] viewportConfigurationTypes;

    // Get the viewport configuration info for the chosen viewport configuration type.
    shockState.ViewportConfig.type = XR_TYPE_VIEW_CONFIGURATION_PROPERTIES;
    xrGetViewConfigurationProperties(shockState.Instance, shockState.SystemId, supportedViewConfigType,
            &shockState.ViewportConfig);

    bool stageSupported = false;
    uint32_t numOutputSpaces = 0;
    xrEnumerateReferenceSpaces(shockState.Session, 0, &numOutputSpaces, NULL);

    XrReferenceSpaceType* referenceSpaces = new XrReferenceSpaceType[numOutputSpaces];
    xrEnumerateReferenceSpaces(shockState.Session, numOutputSpaces, &numOutputSpaces, referenceSpaces);
    for (uint32_t i = 0; i < numOutputSpaces; i++) {
        if (referenceSpaces[i] == XR_REFERENCE_SPACE_TYPE_STAGE) {
            stageSupported = true;
            break;
        }
    }
    delete[] referenceSpaces;

    // Create a space to the first path
    XrReferenceSpaceCreateInfo spaceCreateInfo = {};
    spaceCreateInfo.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
    spaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
    spaceCreateInfo.poseInReferenceSpace.orientation.w = 1.0f;
    xrCreateReferenceSpace(shockState.Session, &spaceCreateInfo, &shockState.HeadSpace);

    spaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
    xrCreateReferenceSpace(shockState.Session, &spaceCreateInfo, &shockState.LocalSpace);

    if (stageSupported) {
        spaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
        spaceCreateInfo.poseInReferenceSpace.position.y = 0.0f;
    } else {
        spaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
        spaceCreateInfo.poseInReferenceSpace.position.y = -1.75f;
    }
    xrCreateReferenceSpace(shockState.Session, &spaceCreateInfo, &shockState.StageSpace);

    return 0;
}

void ShutdownOpenXR() {
    eglMakeCurrent(shockState.EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(shockState.EglDisplay, shockState.EglContext);
    eglTerminate(shockState.EglDisplay);

    xrDestroySpace(shockState.HeadSpace);
    xrDestroySpace(shockState.LocalSpace);
    xrDestroySpace(shockState.StageSpace);

    xrDestroySession(shockState.Session);
    xrDestroyInstance(shockState.Instance);
}

void HandleXrEvents() {
    XrEventDataBuffer eventDataBuffer = {};

    for (;;) {
        XrEventDataBaseHeader* baseEventHeader = (XrEventDataBaseHeader*)(&eventDataBuffer);
        baseEventHeader->type = XR_TYPE_EVENT_DATA_BUFFER;
        baseEventHeader->next = NULL;

        XrResult xrResult;
        xrResult = xrPollEvent(shockState.Instance, &eventDataBuffer);
        if (xrResult != XR_SUCCESS) {
            break;
        }

        switch (baseEventHeader->type) {
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                const XrEventDataSessionStateChanged* session_state_changed_event =
                        (XrEventDataSessionStateChanged*)(baseEventHeader);
                switch (session_state_changed_event->state) {
                    case XR_SESSION_STATE_FOCUSED:
                        shockState.Focused = true;
                        break;
                    case XR_SESSION_STATE_VISIBLE:
                        shockState.Focused = false;
                        break;
                    case XR_SESSION_STATE_READY: {
                        XrSessionBeginInfo sessionBeginInfo = {};
                        sessionBeginInfo.type = XR_TYPE_SESSION_BEGIN_INFO;
                        sessionBeginInfo.next = nullptr;
                        sessionBeginInfo.primaryViewConfigurationType = shockState.ViewportConfig.viewConfigurationType;

                        XrResult xrResult = xrBeginSession(shockState.Session, &sessionBeginInfo);
                        shockState.SessionActive = (xrResult == XR_SUCCESS);
                    } break;
                    case XR_SESSION_STATE_STOPPING: {
                        xrEndSession(shockState.Session);
                        shockState.SessionActive = false;
                    } break;
                    default:
                        break;
                }
            } break;
            default:
                break;
        }
    }
}
