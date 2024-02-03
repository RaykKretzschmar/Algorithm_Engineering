cmake_minimum_required(VERSION 3.9)
project(quantize LANGUAGES C)

set(QUANTIZE_SOURCES
    blur.c
    kmeans.c
    libimagequant.c
    mediancut.c
    mempool.c
    nearest.c
    pam.c
    blur.h
    kmeans.h
    libimagequant.h
    mediancut.h
    mempool.h
    nearest.h
    pam.h)

add_library(quantize STATIC ${QUANTIZE_SOURCES})

target_include_directories(quantize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
