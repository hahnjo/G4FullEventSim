// SPDX-License-Identifier: Apache-2.0

#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // Expect the user to issue "/persistency/gdml/read". If Geant4 calls us here,
  // this didn't happen.
  G4Exception("DetectorConstruction::Construct", "0000", FatalException,
              "Geometry was not loaded via /persistency/gdml/read !");
  return nullptr;
}
