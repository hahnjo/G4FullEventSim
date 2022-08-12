// SPDX-License-Identifier: Apache-2.0

#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4ThreeVector.hh>
#include <G4VUserDetectorConstruction.hh>

#include <memory>
#include <string>

class DetectorConstructionMessenger;

class DetectorConstruction final : public G4VUserDetectorConstruction {
  friend class DetectorConstructionMessenger;

public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;
  void ConstructSDandField() override;

private:
  std::unique_ptr<DetectorConstructionMessenger> fMessenger;

  std::string fGDMLfile;
  G4ThreeVector fMagneticField;
};

#endif // DETECTOR_CONSTRUCTION_HH
