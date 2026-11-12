cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(BotPlay4U VERSION 0.1.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Geode's generated iOS bindings reference FMOD types without including the header.
if (("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS) AND TARGET GeodeBindings)
    target_compile_options(GeodeBindings PRIVATE -include Geode/fmod/fmod.hpp)
endif()

# Sign the iOS dylib so iOS launchers that require a signature can load it.
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND codesign --force --sign - $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Signing the iOS dylib (ad-hoc)")
endif()

setup_geode_mod(${PROJECT_NAME})
