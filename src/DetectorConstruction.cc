// SPDX-License-Identifier: Apache-2.0

#include "DetectorConstruction.hh"

#include <G4Exception.hh>
#include <G4FieldManager.hh>
#include <G4GDMLParser.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4TransportationManager.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>
#include <G4UniformMagField.hh>
#include <G4ios.hh>

class DetectorConstructionMessenger final : public G4UImessenger {
public:
  DetectorConstructionMessenger(DetectorConstruction *detectorConstruction);

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  DetectorConstruction *fDetectorConstruction;

  std::unique_ptr<G4UIdirectory> fDirectory;
  std::unique_ptr<G4UIcmdWithAString> fGDMLfile;
  std::unique_ptr<G4UIcmdWith3VectorAndUnit> fMagneticField;
};

DetectorConstructionMessenger::DetectorConstructionMessenger(
    DetectorConstruction *detectorConstruction)
    : fDetectorConstruction(detectorConstruction) {
  fDirectory.reset(new G4UIdirectory("/detector/"));
  fDirectory->SetGuidance("Set detector parameters.");

  fGDMLfile.reset(new G4UIcmdWithAString("/detector/gdml", this));
  fGDMLfile->SetGuidance("Read geometry from GDML file.");

  fMagneticField.reset(new G4UIcmdWith3VectorAndUnit("/detector/field", this));
  fMagneticField->SetGuidance("Set the constant magenetic field vector.");
  fMagneticField->SetUnitCategory("Magnetic flux density");
  fMagneticField->SetDefaultUnit("tesla");
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command,
                                                G4String newValue) {
  if (command == fGDMLfile.get()) {
    fDetectorConstruction->fGDMLfile = newValue;
  } else if (command == fMagneticField.get()) {
    G4ThreeVector magneticField = fMagneticField->GetNew3VectorValue(newValue);
    fDetectorConstruction->fMagneticField = magneticField;
  }
}

DetectorConstruction::DetectorConstruction() {
  fMessenger.reset(new DetectorConstructionMessenger(this));
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  if (fGDMLfile.empty()) {
    G4Exception("DetectorConstruction::Construct", "0000", FatalException,
                "GDML file was not set via /detector/gdml !");
    return nullptr;
  }

  G4GDMLParser parser;
  parser.Read(fGDMLfile, /*Validate=*/false);
  return parser.GetWorldVolume();
}

void DetectorConstruction::ConstructSDandField() {
  G4cout << G4endl;
  if (fMagneticField == G4ThreeVector()) {
    G4cout << " *** No field set *** " << G4endl;
  } else {
    G4cout << " *** Setting constant magnetic field: [ ";
    G4cout << fMagneticField.x() / tesla << ", " << fMagneticField.y() / tesla;
    G4cout << ", " << fMagneticField.z() / tesla << " ] T *** " << G4endl;

    auto field = new G4UniformMagField(fMagneticField);
    G4FieldManager *fieldManager =
        G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldManager->SetDetectorField(field);
    fieldManager->CreateChordFinder(field);
  }
}
