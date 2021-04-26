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

find_package(HepMC3 QUIET)
if(HepMC3_FOUND)
  message(STATUS "Found HepMC3 version ${HEPMC3_VERSION}: ${HEPMC3_INCLUDE_DIR}")
else()
  message(WARNING "Could not find HepMC3, related functionality will be disabled")
endif()

# Directories
include_directories(include)
add_subdirectory(src)
