// SPDX-License-Identifier: Apache-2.0

#include "GeneratorSettings.hh"

#include "config.hh"

#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>

#if HAVE_HEPMC3
#include <HepMC3/GenEvent.h>
#include <HepMC3/ReaderAscii.h>
#endif

GeneratorSettingsMessenger::GeneratorSettingsMessenger() {
  fDirectory.reset(new G4UIdirectory("/generator/"));
  fDirectory->SetGuidance("Set generator parameters.");

#if HAVE_HEPMC3
  fHepMCfile.reset(new G4UIcmdWithAString("/generator/hepMCfile", this));
  fHepMCfile->SetGuidance("Read event from HepMC3 file.");
#endif

  fEtaCut.reset(new G4UIcommand("/generator/etaCut", this));
  fEtaCut->SetGuidance("Set eta cut of primary vertices.");
  fEtaCut->SetParameter(new G4UIparameter("minEta", 'd', false));
  fEtaCut->SetParameter(new G4UIparameter("maxEta", 'd', false));

  fEnergyCut.reset(new G4UIcmdWithADoubleAndUnit("/generator/energyCut", this));
  fEnergyCut->SetGuidance("Set minimum energy of primary particles.");
  fEnergyCut->SetDefaultUnit("MeV");
  fEnergyCut->SetUnitCategory("Energy");
}

GeneratorSettingsMessenger::~GeneratorSettingsMessenger() {
#if HAVE_HEPMC3
  for (const HepMC3::GenEvent *event : fSettings.events) {
    delete event;
  }
#endif
}

void GeneratorSettingsMessenger::SetNewValue(G4UIcommand *command,
                                             G4String newValue) {
#if HAVE_HEPMC3
  if (command == fHepMCfile.get()) {
    fSettings.events.clear();

    // Read all events.
    HepMC3::ReaderAscii reader(newValue);
    while (!reader.failed()) {
      std::unique_ptr<HepMC3::GenEvent> event(new HepMC3::GenEvent);
      if (!reader.read_event(*event) || reader.failed()) {
        // No more event, the unique_ptr will free the memory.
        break;
      }
      fSettings.events.push_back(event.release());
    }

    size_t numEvents = fSettings.events.size();
    if (numEvents == 0) {
      G4Exception("GeneratorSettingsMessenger::SetNewValue", "0001",
                  FatalException, "Could not load HepMC3 file!");
    }

    G4cout << "Read " << fSettings.events.size() << " events from " << newValue
           << G4endl << G4endl;
  }
#endif

  if (command == fEtaCut.get()) {
    auto pos = newValue.find(" ");
    if (pos != std::string::npos) {
      fSettings.minEta =
          G4UIcommand::ConvertToDouble(newValue.substr(0, pos).c_str());
      fSettings.maxEta =
          G4UIcommand::ConvertToDouble(newValue.substr(pos + 1).c_str());
    }
  } else if (command == fEnergyCut.get()) {
    fSettings.minEnergy = fEnergyCut->GetNewDoubleValue(newValue);
  }
}
