cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

idf_build_set_property(MINIMAL_BUILD ON)
set(CMAKE_CXX_STANDARD 23)

# Awesome, apparently I can't optimize this app for smallest size, because
# Espressif in their genius couldn't implement a proper compiler, so when
# I try to use this level of optimization, it simply breaks the build :D
# 
# If they ever fix this mess that they call a compiler, I'try compiling it
# properly optimized again. For now it'll remain a big executable.
# 
# if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release")
    # idf_build_set_property(CMAKE_C_FLAGS "-Os" TYPE FORCE APPEND)
    # idf_build_set_property(CMAKE_CXX_FLAGS "-Os" TYPE FORCE APPEND)

    # idf_build_set_property(COMPILE_OPTIONS "-flto" TYPE APPEND)
    # idf_build_set_property(LINK_OPTIONS "-flto" TYPE APPEND)
# endif()

project(Tripteron)
