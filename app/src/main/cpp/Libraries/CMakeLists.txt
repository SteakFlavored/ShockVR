set(LIBRARIES_DIR ${PROJECT_SOURCE_DIR}/Libraries)
include_directories(
    ${LIBRARIES_DIR}/DSTRUCT
    ${LIBRARIES_DIR}/FIX
    ${LIBRARIES_DIR}/FIXPP
    ${LIBRARIES_DIR}/H
    ${LIBRARIES_DIR}/LG
    ${LIBRARIES_DIR}/RES
)

set(DSTRUCT_LIBRARY_SRC
    DSTRUCT/array.c
    DSTRUCT/hash.c
    DSTRUCT/llist.c
    DSTRUCT/lllist.c
    DSTRUCT/pqueue.c
    DSTRUCT/rect.c
)
add_library(DSTRUCT_LIB ${DSTRUCT_LIBRARY_SRC})

set(FIX_LIBRARY_SRC
    FIX/f_exp.c
    FIX/fix_sqrt.c
    FIX/fix.c
    FIX/fixsprnt.c
    FIX/maketables.c
)
add_library(FIX_LIB ${FIX_LIBRARY_SRC})

set(FIXPP_LIBRARY_SRC
    FIXPP/fixpp.cpp
)
add_library(FIXPP_LIB ${FIXPP_LIBRARY_SRC})

set(LG_LIBRARY_SRC
    LG/lgsprntf.c
    LG/memall.c
    LG/stack.c
    LG/tmpalloc.c
)
add_library(LG_LIB ${LG_LIBRARY_SRC})

set (RES_LIBRARY_SRC
    RES/lzw.c
    RES/refacc.c
    RES/res.c
    RES/resacc.c
    RES/resbuild.c
    RES/resfile.c
    RES/resload.c
    RES/resmake.c
    RES/restypes.c
)
add_library(RES_LIB ${RES_LIBRARY_SRC})
