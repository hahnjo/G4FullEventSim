# G4FullEventSim

A playground application to simulate a full HepMC3 event and study the generated secondary particles.

## Build Requirements

 * CMake >= 3.8
 * Geant4 10.7 patch-01 with `GEANT4_USE_GDML=ON` (older versions might work)
 * HepMC3 (optional, but highly recommended; used to load events)
 * Pythia8 (optional, for generating new events; some events are stored in the repository)

## Usage Guide

The build system compiles `bin/g4FullEventSim`, which expects a Geant4 macro as its first argument.
An example is given in `cms2018_gg2ttbar.mac`, the file `gg2ttbar.dat` is stored in the `pythia8/` subdirectory.
