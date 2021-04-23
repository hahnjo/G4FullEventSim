// SPDX-License-Identifier: Apache-2.0

#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4GDMLParser.hh>
#include <G4VUserDetectorConstruction.hh>

class DetectorConstruction final : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume *Construct() override;

private:
  G4GDMLParser fParser;
};

#endif // DETECTOR_CONSTRUCTION_HH
