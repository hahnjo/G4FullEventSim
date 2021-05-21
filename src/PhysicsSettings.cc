// SPDX-License-Identifier: Apache-2.0

#include "PhysicsSettings.hh"

#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>
#include <G4UIparameter.hh>

#include <string>

PhysicsSettingsMessenger::PhysicsSettingsMessenger() {
  fMaxTrackTime.reset(new G4UIcmdWithADoubleAndUnit("/maxTrackTime", this));
  fMaxTrackTime->SetGuidance("Set the maximum track time.");
  fMaxTrackTime->SetDefaultUnit("ns");
  fMaxTrackTime->SetUnitCategory("Time");

  fRussianRoulette.reset(new G4UIdirectory("/russianRoulette/"));
  fRussianRoulette->SetGuidance("Set parameters for Roussian Roulette.");

  fRRgamma.reset(new G4UIcommand("/russianRoulette/gamma", this));
  fRRgamma->SetGuidance("Set roulette parameters for gammas.");

  fRRneutron.reset(new G4UIcommand("/russianRoulette/neutron", this));
  fRRneutron->SetGuidance("Set roulette parameters for neutrons.");

  for (auto cmd : {fRRgamma.get(), fRRneutron.get()}) {
    G4UIparameter *prob = new G4UIparameter("prob", 'd', false);
    prob->SetParameterRange("prob>0. && prob<=1.");
    cmd->SetParameter(prob);

    G4UIparameter *limit = new G4UIparameter("limit", 'd', false);
    limit->SetParameterRange("limit>0.");
    cmd->SetParameter(limit);

    G4UIparameter *unit = new G4UIparameter("unit", 's', true);
    unit->SetDefaultValue("MeV");
    cmd->SetParameter(unit);
  }
}

PhysicsSettingsMessenger::~PhysicsSettingsMessenger() {}

void PhysicsSettingsMessenger::SetNewValue(G4UIcommand *command,
                                           G4String newValue) {
  if (command == fMaxTrackTime.get()) {
    fSettings.maxTrackTime = fMaxTrackTime->GetNewDoubleValue(newValue);
  } else if (command == fRRgamma.get() || command == fRRneutron.get()) {
    auto pos = newValue.find(" ");
    if (pos != std::string::npos) {
      double prob =
          G4UIcommand::ConvertToDouble(newValue.substr(0, pos).c_str());
      double limit = G4UIcommand::ConvertToDimensionedDouble(
          newValue.substr(pos + 1).c_str());

      if (command == fRRgamma.get()) {
        fSettings.rrGamma.prob = prob;
        fSettings.rrGamma.limit = limit;
      } else if (command == fRRneutron.get()) {
        fSettings.rrNeutron.prob = prob;
        fSettings.rrNeutron.limit = limit;
      }
    }
  }
}
