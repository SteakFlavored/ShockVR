# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.
project("shockvr")

# -Werror now and forever
set(CMAKE_C_FLAGS, "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# build native_app_glue as a static lib
add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Split out building game libraries separately
set(LIBRARIES_DIR ${PROJECT_SOURCE_DIR}/Libraries)
add_subdirectory(${LIBRARIES_DIR})
include_directories(
    ${LIBRARIES_DIR}/2D
    ${LIBRARIES_DIR}/2D/Clip
    ${LIBRARIES_DIR}/2D/Flat8
    ${LIBRARIES_DIR}/2D/Gen
    ${LIBRARIES_DIR}/2D/GR
    ${LIBRARIES_DIR}/2D/RSD
    ${LIBRARIES_DIR}/3D
    ${LIBRARIES_DIR}/DSTRUCT
    ${LIBRARIES_DIR}/FIX
    ${LIBRARIES_DIR}/FIXPP
    ${LIBRARIES_DIR}/H
    ${LIBRARIES_DIR}/LG
    ${LIBRARIES_DIR}/RES
    ${LIBRARIES_DIR}/RND
)

# We need some of the headers from the Quest directory for the game build
include_directories(Quest)

# Build game code
set(GAME_DIR ${PROJECT_SOURCE_DIR}/Game)
include_directories(${GAME_DIR}/Headers)
set(GAME_SRC
    ${GAME_DIR}/criterr.c
    ${GAME_DIR}/gamestrn.c
)

add_library(shockvr SHARED
    ${GAME_SRC}
    Quest/filesystem.cpp
    Quest/framebuffer.cpp
    Quest/openxr.cpp
    Quest/shockvr.cpp
    android_main.cpp
)

target_link_libraries(shockvr
    EGL
    GLESv3
    2D_LIB
    3D_LIB
    DSTRUCT_LIB
    FIX_LIB
    FIXPP_LIB
    LG_LIB
    PALETTE_LIB
    RES_LIB
    RND_LIB
)

# Link in the native app glue
target_link_libraries(shockvr android native_app_glue)
target_include_directories(shockvr PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

find_library(log-lib log)
target_link_libraries(shockvr ${log-lib})

# Set the absolute path of external dependencies
set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/../../../external)

# Link in the OpenXR loader
set(OPENXR_DIR ${EXTERNAL_DIR}/OpenXR)
add_library(openxr_loader SHARED IMPORTED)
set_property(
        TARGET
        openxr_loader
        PROPERTY
        IMPORTED_LOCATION
        ${OPENXR_DIR}/libs/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}/libopenxr_loader.so
)
target_include_directories(shockvr PRIVATE ${OPENXR_DIR}/include)
target_link_libraries(shockvr openxr_loader)
