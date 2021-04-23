# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.8...3.18 FATAL_ERROR)
project(G4FullEventSim)

# Settings
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Dependencies
find_package(Geant4 REQUIRED gdml)
message(STATUS "Found Geant4 version ${Geant4_VERSION}: ${Geant4_INCLUDE_DIRS}")

# Directories
include_directories(include)
add_subdirectory(src)
