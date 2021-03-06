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

set(2D_LIBRARY_SRC
    2D/bit.c
    2D/bitmap.c
    2D/blend.c
    2D/buffer.c
    2D/canvas.c
    2D/chain.c
    2D/chnfuncs.c
    2D/chrsiz.c
    2D/chrwid.c
    2D/close.c
    2D/cnvtab.c
    2D/context.c
    2D/detect.c
    2D/devtab.c
    2D/fcntab.c
    2D/init.c
    2D/invtab.c
    2D/lintab.c
    2D/mode.c
    2D/pal.c
    2D/permap.c
    2D/persetup.c
    2D/pertol.c
    2D/pixfill.c
    2D/rgb.c
    2D/screen.c
    2D/sscrn.c
    2D/statestk.c
    2D/strnsiz.c
    2D/strscl.c
    2D/struscl.c
    2D/strwid.c
    2D/strwrap.c
    2D/svgainit.c
    2D/tempbm.c
    2D/temptm.c
    2D/tlucdat.c
    2D/tluctab.c
    2D/valloc.c
    2D/vtab.c
    2D/wirpoly.c
    2D/Clip/clpclin.c
    2D/Clip/clpf24.c
    2D/Clip/clplin.c
    2D/Clip/clplin2.c
    2D/Clip/clpltab.c
    2D/Clip/clpmono.c
    2D/Clip/clpply.c
    2D/Clip/clppoly.c
    2D/Clip/clprect.c
    2D/Clip/clpslin.c
    2D/Flat8/fl8bl.c
    2D/Flat8/fl8bldbl.c
    2D/Flat8/fl8chfl8.c
    2D/Flat8/fl8clear.c
    2D/Flat8/fl8clin.c
    2D/Flat8/fl8cnv.c
    2D/Flat8/fl8cop.c
    2D/Flat8/fl8cply.c
    2D/Flat8/fl8ctp.c
    2D/Flat8/fl8dbl.c
    2D/Flat8/fl8f.c
    2D/Flat8/fl8fl8.c
    2D/Flat8/fl8fl8c.c
    2D/Flat8/fl8fl8m.c
    2D/Flat8/fl8fltr2.c
    2D/Flat8/fl8ft.c
    2D/Flat8/fl8g24.c
    2D/Flat8/fl8gfl8.c
    2D/Flat8/fl8gpix.c
    2D/Flat8/fl8hfl8.c
    2D/Flat8/fl8hlin.c
    2D/Flat8/fl8lf.c
    2D/Flat8/fl8lin.c
    2D/Flat8/fl8ll.c
    2D/Flat8/fl8lnop.c
    2D/Flat8/fl8lop.c
    2D/Flat8/fl8lp.c
    2D/Flat8/fl8ltp.c
    2D/Flat8/fl8lw.c
    2D/Flat8/fl8mono.c
    2D/Flat8/fl8mscl.c
    2D/Flat8/fl8nl.c
    2D/Flat8/fl8ns.c
    2D/Flat8/fl8ntrp2.c
    2D/Flat8/fl8opl.c
    2D/Flat8/fl8p.c
    2D/Flat8/fl8p24.c
    2D/Flat8/fl8pix.c
    2D/Flat8/fl8ply.c
    2D/Flat8/fl8pnt.c
    2D/Flat8/fl8rect.c
    2D/Flat8/fl8row.c
    2D/Flat8/fl8rsd8.c
    2D/Flat8/fl8s.c
    2D/Flat8/fl8slin.c
    2D/Flat8/fl8sub.c
    2D/Flat8/fl8tl8.c
    2D/Flat8/fl8tpl.c
    2D/Flat8/fl8tsmap.c
    2D/Flat8/fl8vlin.c
    2D/Flat8/fl8w.c
    2D/Flat8/fl8wclin.c
    2D/Flat8/fl8wlin.c
    2D/Gen/genbox.c
    2D/Gen/genchfl8.c
    2D/Gen/genchr.c
    2D/Gen/gencirc.c
    2D/Gen/genclin.c
    2D/Gen/gencnv.c
    2D/Gen/gencwlin.c
    2D/Gen/gendisk.c
    2D/Gen/genel.c
    2D/Gen/general.c
    2D/Gen/genf24.c
    2D/Gen/genfl8.c
    2D/Gen/genfl8c.c
    2D/Gen/gengfl8.c
    2D/Gen/genhfl8.c
    2D/Gen/genhlin.c
    2D/Gen/genlin.c
    2D/Gen/genmono.c
    2D/Gen/genov.c
    2D/Gen/genpix.c
    2D/Gen/genrect.c
    2D/Gen/genrsd8.c
    2D/Gen/genrsdbm.c
    2D/Gen/genrsdtm.c
    2D/Gen/genslin.c
    2D/Gen/genstr.c
    2D/Gen/gente.c
    2D/Gen/gentl8.c
    2D/Gen/gentm.c
    2D/Gen/genuchr.c
    2D/Gen/genuclin.c
    2D/Gen/genuhlin.c
    2D/Gen/genulin.c
    2D/Gen/genuslin.c
    2D/Gen/genustr.c
    2D/Gen/genuvlin.c
    2D/Gen/genvcply.c
    2D/Gen/genvlin.c
    2D/Gen/genvpoly.c
    2D/Gen/genvrect.c
    2D/Gen/genwclin.c
    2D/Gen/genwlin.c
    2D/GR/grd.c
    2D/GR/grilin.c
    2D/GR/grnull.c
    2D/GR/gruilin.c
    2D/RSD/rsdcvt.c
    2D/RSD/rsdunpack.c
)
add_library(2D_LIB ${2D_LIBRARY_SRC})
target_link_libraries(2D_LIB FIX_LIB LG_LIB)

set(3D_LIBRARY_SRC
    3D/alloc.c
    3D/bitmap.c
    3D/clip.c
    3D/detail.c
    3D/fov.c
    3D/globalv.c
    3D/instance.c
    3D/interp.c
    3D/light.c
    3D/matrix.c
    3D/points.c
    3D/polygon.c
    3D/slew.c
    3D/tmap.c
    3D/vector.c
)
add_library(3D_LIB ${3D_LIBRARY_SRC})
target_link_libraries(3D_LIB FIX_LIB LG_LIB)

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

set(PALETTE_LIBRARY_SRC PALETTE/palette.c)
add_library(PALETTE_LIB ${PALETTE_LIBRARY_SRC})

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

set(RND_LIBRARY_SRC RND/rnd.c)
add_library(RND_LIB ${RND_LIBRARY_SRC})
target_link_libraries(RND_LIB FIX_LIB)
