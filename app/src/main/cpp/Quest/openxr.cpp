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

#include <cstring>
#include <vector>

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
            XR_KHR_VULKAN_ENABLE_EXTENSION_NAME,
            XR_KHR_COMPOSITION_LAYER_CYLINDER_EXTENSION_NAME,
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
    PFN_xrGetVulkanGraphicsRequirementsKHR pfnGetVulkanGraphicsRequirementsKHR = nullptr;
    xrResult = xrGetInstanceProcAddr(shockState.Instance, "xrGetVulkanGraphicsRequirementsKHR",
            (PFN_xrVoidFunction*)(&pfnGetVulkanGraphicsRequirementsKHR));
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get xrGetVulkanGraphicsRequirementsKHR function pointer.");
        return 1;
    }

    XrGraphicsRequirementsVulkanKHR graphicsRequirements = {};
    graphicsRequirements.type = XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR;
    pfnGetVulkanGraphicsRequirementsKHR(shockState.Instance, shockState.SystemId, &graphicsRequirements);

    // Get the list of required Vulkan extensions
    PFN_xrGetVulkanInstanceExtensionsKHR pfnGetVulkanInstanceExtensionsKHR = nullptr;
    xrGetInstanceProcAddr(shockState.Instance, "xrGetVulkanInstanceExtensionsKHR",
            (PFN_xrVoidFunction*)&pfnGetVulkanInstanceExtensionsKHR);

    uint32_t namesCapacity = 0;
    xrResult = pfnGetVulkanInstanceExtensionsKHR(shockState.Instance, shockState.SystemId, 0, &namesCapacity, nullptr);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get required Vulkan extensions: %d.", xrResult);
        return 1;
    }

    char* instanceExtensionNames = new char[namesCapacity];
    xrResult = pfnGetVulkanInstanceExtensionsKHR(shockState.Instance, shockState.SystemId, namesCapacity,
            &namesCapacity, instanceExtensionNames);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get required Vulkan extensions: %d.", xrResult);
        return 1;
    }

    std::vector<const char *> requiredInstanceExtensions;
    char *token = std::strtok(instanceExtensionNames, " ");
    while (token != nullptr) {
        requiredInstanceExtensions.push_back(token);
        token = std::strtok(nullptr, " ");
    }

    // Create a Vulkan instance.
    VkApplicationInfo vkAppInfo{};
    vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vkAppInfo.pApplicationName = "ShockVR";
    vkAppInfo.applicationVersion = 1;
    vkAppInfo.pEngineName = "System Shock VR";
    vkAppInfo.engineVersion = 1;
    vkAppInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo vkInstanceCreateInfo{};
    vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkInstanceCreateInfo.pApplicationInfo = &vkAppInfo;
    vkInstanceCreateInfo.enabledExtensionCount = requiredInstanceExtensions.size();
    vkInstanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions.data();

    VkResult vkResult = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &shockState.VkInstance);
    delete[] instanceExtensionNames;

    if (vkResult != VK_SUCCESS) {
        LOGE("Failed to create Vulkan instance: %d", vkResult);
        return 1;
    }

    // Get the physical device for the Vulkan instance.
    PFN_xrGetVulkanGraphicsDeviceKHR pfnGetVulkanGraphicsDeviceKHR = nullptr;
    xrResult = xrGetInstanceProcAddr(shockState.Instance, "xrGetVulkanGraphicsDeviceKHR",
            (PFN_xrVoidFunction*)&pfnGetVulkanGraphicsDeviceKHR);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get xrGetVulkanGraphicsDeviceKHR function pointer: %d", xrResult);
        return 1;
    }

    pfnGetVulkanGraphicsDeviceKHR(shockState.Instance, shockState.SystemId, shockState.VkInstance,
            &shockState.PhysicalDevice);
    if (shockState.PhysicalDevice == VK_NULL_HANDLE) {
        LOGE("Failed to get Vulkan physical device.");
        return 1;
    }

    vkGetPhysicalDeviceFeatures(shockState.PhysicalDevice, &shockState.PhysicalDeviceFeatures);
    vkGetPhysicalDeviceProperties(shockState.PhysicalDevice, &shockState.PhysicalDeviceProperties);
    vkGetPhysicalDeviceMemoryProperties(shockState.PhysicalDevice, &shockState.PhysicalDeviceMemoryProperties);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(shockState.PhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(shockState.PhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    uint32_t queueFamilyIndex;
    for (queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }

    PFN_xrGetVulkanDeviceExtensionsKHR pfnGetVulkanDeviceExtensionsKHR = nullptr;
    xrResult = xrGetInstanceProcAddr(shockState.Instance, "xrGetVulkanDeviceExtensionsKHR",
            (PFN_xrVoidFunction*)&pfnGetVulkanDeviceExtensionsKHR);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get xrGetVulkanDeviceExtensionsKHR function pointer: %d", xrResult);
        return 1;
    }

    namesCapacity = 0;
    pfnGetVulkanDeviceExtensionsKHR(shockState.Instance, shockState.SystemId, 0, &namesCapacity, nullptr);

    char* deviceExtensionNames = new char[namesCapacity];
    xrResult = pfnGetVulkanDeviceExtensionsKHR(shockState.Instance, shockState.SystemId, namesCapacity,
            &namesCapacity, deviceExtensionNames);
    if (xrResult != XR_SUCCESS) {
        LOGE("Failed to get required Vulkan device extensions: %d.", xrResult);
        return 1;
    }

    std::vector<const char *> requiredDeviceExtensions;
    token = std::strtok(deviceExtensionNames, " ");
    while (token != nullptr) {
        requiredDeviceExtensions.push_back(token);
        token = std::strtok(nullptr, " ");
    }

    // Create the device.
    VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    deviceQueueCreateInfo.queueCount = 1;
    const float queuePriority = 1.0f;
    deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = requiredDeviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();

    vkResult = vkCreateDevice(shockState.PhysicalDevice, &deviceCreateInfo, nullptr, &shockState.Device);
    delete[] deviceExtensionNames;

    if (vkResult != VK_SUCCESS) {
        LOGE("Failed to create Vulkan device: %d", vkResult);
        return 1;
    }

    // Finish assembling the Vulkan context.
    vkGetDeviceQueue(shockState.Device, queueFamilyIndex, 0, &shockState.Queue);

    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

    vkResult = vkCreateCommandPool(shockState.Device, &commandPoolCreateInfo, nullptr, &shockState.CommandPool);
    if (vkResult != VK_SUCCESS) {
        LOGE("Failed to create Vulkan command pool: %d", vkResult);
        return 1;
    }

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    vkResult = vkCreatePipelineCache(shockState.Device, &pipelineCacheCreateInfo, nullptr, &shockState.PipelineCache);
    if (vkResult != VK_SUCCESS) {
        LOGE("Failed to create Vulkan pipeline cache: %d", vkResult);
        return 1;
    }

    // Create the OpenXR Session.
    XrGraphicsBindingVulkanKHR graphicsBindingVulkan{};
    graphicsBindingVulkan.type = XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR;
    graphicsBindingVulkan.instance = shockState.VkInstance;
    graphicsBindingVulkan.physicalDevice = shockState.PhysicalDevice;
    graphicsBindingVulkan.device = shockState.Device;
    graphicsBindingVulkan.queueFamilyIndex = queueFamilyIndex;
    graphicsBindingVulkan.queueIndex = 0;

    XrSessionCreateInfo sessionCreateInfo{};
    sessionCreateInfo.type = XR_TYPE_SESSION_CREATE_INFO;
    sessionCreateInfo.next = &graphicsBindingVulkan;
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
    xrDestroySpace(shockState.HeadSpace);
    xrDestroySpace(shockState.LocalSpace);
    xrDestroySpace(shockState.StageSpace);

    vkDestroyCommandPool(shockState.Device, shockState.CommandPool, nullptr);
    vkDestroyPipelineCache(shockState.Device, shockState.PipelineCache, nullptr);
    vkDestroyDevice(shockState.Device, nullptr);
    vkDestroyInstance(shockState.VkInstance, nullptr);

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
