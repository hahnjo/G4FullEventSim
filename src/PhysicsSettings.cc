// SPDX-License-Identifier: Apache-2.0

#include "PhysicsSettings.hh"

#include <G4UIcmdWithADoubleAndUnit.hh>

PhysicsSettingsMessenger::PhysicsSettingsMessenger() {
  fMaxTrackTime.reset(new G4UIcmdWithADoubleAndUnit("/maxTrackTime", this));
  fMaxTrackTime->SetGuidance("Set the maximum track time.");
  fMaxTrackTime->SetDefaultUnit("ns");
  fMaxTrackTime->SetUnitCategory("Time");
}

PhysicsSettingsMessenger::~PhysicsSettingsMessenger() {}

void PhysicsSettingsMessenger::SetNewValue(G4UIcommand *command,
                                           G4String newValue) {
  if (command == fMaxTrackTime.get()) {
    fSettings.maxTrackTime = fMaxTrackTime->GetNewDoubleValue(newValue);
  }
}
