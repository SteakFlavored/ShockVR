set(LIBRARIES_DIR ${PROJECT_SOURCE_DIR}/Libraries)
include_directories(
    ${LIBRARIES_DIR}/FIX
    ${LIBRARIES_DIR}/H
)

set(FIX_LIBRARY_SRC
    FIX/f_exp.c
    FIX/fix_sqrt.c
    FIX/fix.c
    FIX/fixsprnt.c
    FIX/maketables.c
)
add_library(FIX_LIB ${FIX_LIBRARY_SRC})
