set(LIBRARIES_DIR ${PROJECT_SOURCE_DIR}/Libraries)
include_directories(
    ${LIBRARIES_DIR}/FIX
    ${LIBRARIES_DIR}/FIXPP
    ${LIBRARIES_DIR}/H
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
