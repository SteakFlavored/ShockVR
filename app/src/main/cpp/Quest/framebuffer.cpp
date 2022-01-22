/*

Framebuffer handling for the ShockVR project
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

#include "framebuffer.h"
#include "shockvr.h"

#include <android/native_window_jni.h>

struct Swapchain {
    XrSwapchain Handle;
    ANativeWindow *Window;
    uint32_t Width;
    uint32_t Height;
};

extern "C" {

ANativeWindow *CreateSwapchain(const uint32_t width, const uint32_t height) {
    XrSwapchainCreateInfo swapchainCreateInfo;
    memset(&swapchainCreateInfo, 0, sizeof(swapchainCreateInfo));
    swapchainCreateInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
    swapchainCreateInfo.next = nullptr;
    swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.format = 0;
    swapchainCreateInfo.sampleCount = 0;
    swapchainCreateInfo.width = width;
    swapchainCreateInfo.height = height;
    swapchainCreateInfo.faceCount = 0;
    swapchainCreateInfo.arraySize = 0;
    swapchainCreateInfo.mipCount = 0;

    PFN_xrCreateSwapchainAndroidSurfaceKHR pfnCreateSwapchainAndroidSurfaceKHR = nullptr;
    XrResult xrResult = xrGetInstanceProcAddr(shockState.Instance, "xrCreateSwapchainAndroidSurfaceKHR",
            (PFN_xrVoidFunction*)(&pfnCreateSwapchainAndroidSurfaceKHR));
    if (xrResult != XR_SUCCESS || pfnCreateSwapchainAndroidSurfaceKHR == nullptr) {
        LOGE("xrGetInstanceProcAddr failed for xrCreateSwapchainAndroidSurfaceKHR");
        return nullptr;
    }

    XrSwapchain handle = XR_NULL_HANDLE;
    jobject surface = nullptr;

    xrResult = pfnCreateSwapchainAndroidSurfaceKHR(shockState.Session, &swapchainCreateInfo, &handle, &surface);
    if (xrResult != XR_SUCCESS || handle == XR_NULL_HANDLE || surface == nullptr) {
        LOGE("Failed to create Android surface swapchain: %d", xrResult);
        return nullptr;
    }

    Swapchain *swapchain = new Swapchain;
    swapchain->Handle = handle;
    swapchain->Window = ANativeWindow_fromSurface(shockState.Env, surface);
    swapchain->Width = width;
    swapchain->Height = height;

    return swapchain->Window;
}

// Horrific looking function to retrieve the Swapchain that "owns" an ANW, a la Linux's 'container_of'.
Swapchain *SwapchainFromWindow(ANativeWindow *window) {
    return reinterpret_cast<Swapchain*>(reinterpret_cast<intptr_t>(window) -
            reinterpret_cast<ssize_t>(&(static_cast<Swapchain*>(nullptr)->Window)));
}

void DestroySwapchain(ANativeWindow *swapchainWindow) {

    Swapchain *swapchain = SwapchainFromWindow(swapchainWindow);

    if (swapchain->Handle != XR_NULL_HANDLE) {
        xrDestroySwapchain(swapchain->Handle);
    }

    delete swapchain;
}

}
