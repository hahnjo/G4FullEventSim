# SPDX-License-Identifier: Apache-2.0

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
