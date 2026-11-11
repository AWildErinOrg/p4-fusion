cmake_minimum_required(VERSION 3.16)

option(MTR_ENABLED "Enable minitrace profiling" OFF)
option(BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Perforce's static Windows SDK uses /MT (/MTd for Debug).
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# Vendored projects have older cmake_minimum_required() versions.
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

project(
    p4-fusion
    LANGUAGES C CXX
    VERSION 1.0
    DESCRIPTION "Custom tool to convert P4 depots to Git repositories using the Helix Core C++ API"
)

set(BUILD_CLAR OFF)
set(BUILD_SHARED_LIBS OFF)

if (WIN32)
    if (NOT MSVC)
        message(FATAL_ERROR "Windows builds require MSVC and the matching static Perforce SDK.")
    endif()
    set(HELIX_API_DEFAULT vendor/helix-core-api/windows)
    set(STATIC_CRT ON CACHE BOOL "Link the static CRT libraries" FORCE)
elseif (APPLE)
    set(HELIX_API_DEFAULT vendor/helix-core-api/mac)
else()
    set(HELIX_API_DEFAULT vendor/helix-core-api/linux)
endif()
set(HELIX_API "${HELIX_API_DEFAULT}" CACHE PATH "Path to the extracted Helix Core C++ API SDK")
get_filename_component(HELIX_API "${HELIX_API}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

include(cmake/HelixAPI.cmake)

add_subdirectory(vendor)
add_subdirectory(p4-fusion)

if (BUILD_TESTS)
    enable_testing()
    message(STATUS "Building tests")
    add_subdirectory(tests)
endif()
