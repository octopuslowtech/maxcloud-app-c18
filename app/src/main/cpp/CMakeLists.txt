cmake_minimum_required(VERSION 3.22.1)

project(libCloudPhone VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Werror=format -fdata-sections -ffunction-sections -O2 -fvisibility=hidden -fvisibility-inlines-hidden")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--unresolved-symbols=ignore-in-shared-libs -Wl,--hash-style=both -Wl,-exclude-libs,ALL -Wl,--gc-sections")

find_library(log-lib log)
find_library(android-lib android)
find_library(jnigraphics-lib jnigraphics)
find_library(mediandk mediandk)

file(GLOB_RECURSE BASE_SOURCES "${CMAKE_SOURCE_DIR}/Base/*.cpp")
file(GLOB_RECURSE BASE_HEADERS "${CMAKE_SOURCE_DIR}/Base/*.h")

file(GLOB_RECURSE CORE_SOURCES "${CMAKE_SOURCE_DIR}/Core/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "${CMAKE_SOURCE_DIR}/Core/*.h")

# OPENCV
add_library(lib_opencv STATIC IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libopencv_java4.so)

# OPENSSL
add_library(libssl STATIC IMPORTED)
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libssl.so)

add_library(libcrypto STATIC IMPORTED)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libcrypto.so)


# CURL
add_library(libcurl STATIC IMPORTED)
set_target_properties(libcurl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libcurl.so)

# TurboJPEG
add_library(libturbojpeg STATIC IMPORTED)
set_target_properties(libturbojpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libturbojpeg.so)
include_directories(${CMAKE_SOURCE_DIR}/Lib/libturbojpeg)

# OMP
add_library(omp STATIC IMPORTED)
set_target_properties(omp PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libomp.so)

#include directories BASE
set(BASE_INCLUDE_DIRS "")
foreach (HEADER_FILE ${BASE_HEADERS})
    get_filename_component(DIR ${HEADER_FILE} DIRECTORY)
    list(APPEND BASE_INCLUDE_DIRS ${DIR})
endforeach ()

list(REMOVE_DUPLICATES BASE_INCLUDE_DIRS)
include_directories(${BASE_INCLUDE_DIRS})

#include directories Lib
include_directories(${CMAKE_SOURCE_DIR}/Lib)

# Datachannel
add_subdirectory("${CMAKE_SOURCE_DIR}/Lib/libdatachannel" "libdatachannel")


add_library(NativeCore SHARED
        NativeCore.cpp

        ${CORE_SOURCES}
        ${CORE_HEADERS}

        ${BASE_SOURCES}
        ${BASE_HEADERS}

        Lib/cryptor.hpp

        Lib/json.hpp
        Lib/pugiconfig.hpp
        Lib/pugixml.cpp
        Lib/pugixml.hpp

        Lib/zib/miniz.h
        Lib/zib/zip.c
        Lib/zib/zip.h
)

target_link_libraries(NativeCore
        ${log-lib}
        ${android-lib}
        ${jnigraphics-lib}
        ${mediandk}

        datachannel
        lib_opencv
        libssl
        libcrypto
        libcurl
        libturbojpeg
        omp
)