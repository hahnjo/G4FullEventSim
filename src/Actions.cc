// SPDX-License-Identifier: Apache-2.0

#include "Actions.hh"

#include <G4Electron.hh>
#include <G4Gamma.hh>
#include <G4Neutron.hh>
#include <G4Positron.hh>
#include <G4RunManager.hh>
#include <G4Track.hh>

void MasterRunAction::BeginOfRunAction(const G4Run *) { fTimer.Start(); }

void MasterRunAction::EndOfRunAction(const G4Run *aRun) {
  fTimer.Stop();

  // clang-format off
  G4int events = aRun->GetNumberOfEvent();
  G4cout << " ============================================================================== " << G4endl;
  G4cout << "  Run terminated, " << events << " events transported" << G4endl;
  G4cout << "  Time: " << fTimer << G4endl;
  G4cout << " ============================================================================== " << G4endl;
  // clang-format on
  G4cout << G4endl;

  const Run *run = static_cast<const Run *>(aRun);
  const ParticleStatistics &stats = run->GetStatistics();
  double numPrimaries = ((double)stats.numPrimaries) / events;
  double numSecondaries = ((double)stats.numSecondaries) / events;
  double numElectrons = ((double)stats.numElectrons) / events;
  double numPositrons = ((double)stats.numPositrons) / events;
  double numGammas = ((double)stats.numGammas) / events;
  double numNeutrons = ((double)stats.numNeutrons) / events;
  double numOthers = ((double)stats.numOthers) / events;

  G4cout << "Average number of particles per event:" << G4endl;
  G4cout << std::defaultfloat;
  G4cout << "  primaries:   " << numPrimaries << G4endl;
  G4cout << std::scientific;
  G4cout << "  secondaries: " << numSecondaries << G4endl;
  G4cout << "  tracked particles:" << G4endl;
  G4cout << "    electrons: " << numElectrons << G4endl;
  G4cout << "    positrons: " << numPositrons << G4endl;
  G4cout << "    gammas:    " << numGammas << G4endl;
  G4cout << "    neutrons:  " << numNeutrons << G4endl;
  G4cout << "    others:    " << numOthers << G4endl;
}

void EventAction::BeginOfEventAction(const G4Event *) { fStatistics.Clear(); }

void EventAction::EndOfEventAction(const G4Event *) {
  auto *runManager = G4RunManager::GetRunManager();
  Run *run = static_cast<Run *>(runManager->GetNonConstCurrentRun());
  run->AddStatistics(fStatistics);
}

void EventAction::AccountTrack(const G4Track *track) {
  if (track->GetParentID() == 0) {
    fStatistics.numPrimaries++;
  } else {
    fStatistics.numSecondaries++;
  }

  const G4ParticleDefinition *particleDef = track->GetDefinition();
  if (particleDef == G4Electron::Definition()) {
    fStatistics.numElectrons++;
  } else if (particleDef == G4Positron::Definition()) {
    fStatistics.numPositrons++;
  } else if (particleDef == G4Gamma::Definition()) {
    fStatistics.numGammas++;
  } else if (particleDef == G4Neutron::Definition()) {
    fStatistics.numNeutrons++;
  } else {
    fStatistics.numOthers++;
  }
}
