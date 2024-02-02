set(QUANTIZE_SOURCES
    blur.c
    kmeans.c
    libimagequant.c
    mediancut.c
    mempool.c
    nearest.c
    pam.c)

add_library(quantize ${QUANTIZE_SOURCES})

target_include_directories(quantize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
