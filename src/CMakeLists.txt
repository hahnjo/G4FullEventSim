# SPDX-License-Identifier: Apache-2.0

set(HAVE_HEPMC3 ${HepMC3_FOUND})
configure_file(config.hh.in config.hh @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(g4FullEventSim
  main.cc
  ActionInitialization.cc
  Actions.cc
  DetectorConstruction.cc
  PhysicsList.cc
  PrimaryGeneratorAction.cc
)
target_compile_options(g4FullEventSim PRIVATE -Wall -Wextra -Wsuggest-override)
target_link_libraries(g4FullEventSim ${Geant4_LIBRARIES})
if(HepMC3_FOUND)
  target_include_directories(g4FullEventSim PRIVATE ${HEPMC3_INCLUDE_DIR})
  target_link_libraries(g4FullEventSim ${HEPMC3_LIB})
endif()
