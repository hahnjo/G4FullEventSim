// SPDX-License-Identifier: Apache-2.0

#include "OutputSettings.hh"

#include <G4UIcmdWithABool.hh>
#include <G4UIdirectory.hh>

OutputSettingsMessenger::OutputSettingsMessenger() {
  fOutputDir.reset(new G4UIdirectory("/output/"));
  fOutputDir->SetGuidance("Set parameters for the simulation output.");

  fPrintParticleStats.reset(
      new G4UIcmdWithABool("/output/printParticleStats", this));
  fPrintParticleStats->SetGuidance("Print statistics about tracked particles.");
  fPrintParticleStats->SetDefaultValue(true);
  fPrintParticleStats->GetParameter(0)->SetOmittable(true);

  fPrintStepStats.reset(new G4UIcmdWithABool("/output/printStepStats", this));
  fPrintStepStats->SetGuidance("Print statistics about number of steps.");
  fPrintStepStats->SetDefaultValue(true);
  fPrintStepStats->GetParameter(0)->SetOmittable(true);
}

OutputSettingsMessenger::~OutputSettingsMessenger() {}

void OutputSettingsMessenger::SetNewValue(G4UIcommand *command,
                                          G4String newValue) {
  if (command == fPrintParticleStats.get()) {
    fSettings.printParticleStats =
        fPrintParticleStats->GetNewBoolValue(newValue);
  } else if (command == fPrintStepStats.get()) {
    fSettings.printStepStats = fPrintStepStats->GetNewBoolValue(newValue);
  }
}
