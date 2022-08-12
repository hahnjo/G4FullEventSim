// SPDX-License-Identifier: Apache-2.0

#include "PhysicsSettings.hh"

#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>
#include <G4UIparameter.hh>

#include <sstream>
#include <string>

PhysicsSettingsMessenger::PhysicsSettingsMessenger() {
  fMaxTrackTime.reset(new G4UIcmdWithADoubleAndUnit("/maxTrackTime", this));
  fMaxTrackTime->SetGuidance("Set the maximum track time.");
  fMaxTrackTime->SetDefaultUnit("ns");
  fMaxTrackTime->SetUnitCategory("Time");

  fDeadRegion.reset(new G4UIcmdWithAString("/deadRegion", this));
  fDeadRegion->SetGuidance("Add a dead region, all particles will be killed.");

  fRussianRoulette.reset(new G4UIdirectory("/russianRoulette/"));
  fRussianRoulette->SetGuidance("Set parameters for Roussian Roulette.");

  fRRgamma.reset(new G4UIcommand("/russianRoulette/gamma", this));
  fRRgamma->SetGuidance("Set roulette parameters for gammas.");

  fRRneutron.reset(new G4UIcommand("/russianRoulette/neutron", this));
  fRRneutron->SetGuidance("Set roulette parameters for neutrons.");

  for (auto *cmd : {fRRgamma.get(), fRRneutron.get()}) {
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

  fFTFP_BERT.reset(new G4UIdirectory("/FTFP_BERT/"));
  fFTFP_BERT->SetGuidance("Set threshold between BERT and FTFP.");

  fFTFP_BERTpion.reset(new G4UIcommand("/FTFP_BERT/pion", this));
  fFTFP_BERTpion->SetGuidance("Set threshold for pions.");

  fFTFP_BERTkaon.reset(new G4UIcommand("/FTFP_BERT/kaon", this));
  fFTFP_BERTkaon->SetGuidance("Set threshold for kaons.");

  fFTFP_BERTproton.reset(new G4UIcommand("/FTFP_BERT/proton", this));
  fFTFP_BERTproton->SetGuidance("Set threshold for protons.");

  fFTFP_BERTneutron.reset(new G4UIcommand("/FTFP_BERT/neutron", this));
  fFTFP_BERTneutron->SetGuidance("Set threshold for neutrons.");

  for (auto *cmd : {fFTFP_BERTpion.get(), fFTFP_BERTkaon.get(),
                    fFTFP_BERTproton.get(), fFTFP_BERTneutron.get()}) {
    G4UIparameter *minFTFP = new G4UIparameter("minFTFP", 'd', false);
    minFTFP->SetParameterRange("minFTFP>0.");
    cmd->SetParameter(minFTFP);

    G4UIparameter *maxBERT = new G4UIparameter("maxBERT", 'd', false);
    maxBERT->SetParameterRange("maxBERT>0.");
    cmd->SetParameter(maxBERT);

    G4UIparameter *unit = new G4UIparameter("unit", 's', true);
    unit->SetDefaultValue("GeV");
    cmd->SetParameter(unit);
  }
}

PhysicsSettingsMessenger::~PhysicsSettingsMessenger() {}

void PhysicsSettingsMessenger::SetNewValue(G4UIcommand *command,
                                           G4String newValue) {
  if (command == fMaxTrackTime.get()) {
    fSettings.maxTrackTime = fMaxTrackTime->GetNewDoubleValue(newValue);
  } else if (command == fDeadRegion.get()) {
    const G4Region *region =
        G4RegionStore::GetInstance()->GetRegion(newValue, /*verbose=*/false);
    if (region == nullptr) {
      G4Exception("PhysicsSettingsMessenger::SetNewValue", "0003",
                  FatalException, "Could not find region!");
    }
    fSettings.deadRegions.insert(region);
  } else if (command == fRRgamma.get() || command == fRRneutron.get()) {
    auto pos = newValue.find(" ");
    if (pos != std::string::npos) {
      double prob =
          G4UIcommand::ConvertToDouble(newValue.substr(0, pos).c_str());
      double limit = G4UIcommand::ConvertToDimensionedDouble(
          newValue.substr(pos + 1).c_str());

      RouletteParameters *params = nullptr;
      if (command == fRRgamma.get()) {
        params = &fSettings.rrGamma;
      } else if (command == fRRneutron.get()) {
        params = &fSettings.rrNeutron;
      }
      params->prob = prob;
      params->limit = limit;
    }
  } else if (command == fFTFP_BERTpion.get() ||
             command == fFTFP_BERTkaon.get() ||
             command == fFTFP_BERTproton.get() ||
             command == fFTFP_BERTneutron.get()) {
    std::istringstream ss(newValue);
    double minFTFP, maxBERT;
    G4String unit;
    ss >> minFTFP >> maxBERT >> unit;

    double unitMult = G4UIcommand::ValueOf(unit);
    minFTFP *= unitMult;
    maxBERT *= unitMult;

    FTFP_BERTparameters *params = nullptr;
    if (command == fFTFP_BERTpion.get()) {
      params = &fSettings.FTFP_BERTpion;
    } else if (command == fFTFP_BERTkaon.get()) {
      params = &fSettings.FTFP_BERTkaon;
    } else if (command == fFTFP_BERTproton.get()) {
      params = &fSettings.FTFP_BERTproton;
    } else if (command == fFTFP_BERTneutron.get()) {
      params = &fSettings.FTFP_BERTneutron;
    }
    params->minFTFP = minFTFP;
    params->maxBERT = maxBERT;
  }
}
