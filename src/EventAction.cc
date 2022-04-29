// SPDX-License-Identifier: Apache-2.0

#include "EventAction.hh"

#include "ParticleStatistics.hh"
#include "Run.hh"

#include <G4Electron.hh>
#include <G4Gamma.hh>
#include <G4Neutron.hh>
#include <G4Positron.hh>
#include <G4RunManager.hh>
#include <G4Track.hh>

void EventAction::BeginOfEventAction(const G4Event *) {
  fParticleStats.Clear();
}

void EventAction::EndOfEventAction(const G4Event *) {
  auto *runManager = G4RunManager::GetRunManager();
  Run *run = static_cast<Run *>(runManager->GetNonConstCurrentRun());
  run->AddStatistics(fParticleStats);
}

void EventAction::AccountTrack(const G4Track *track) {
  if (track->GetParentID() == 0) {
    fParticleStats.numPrimaries++;
  } else {
    fParticleStats.numSecondaries++;
  }

  const G4ParticleDefinition *particleDef = track->GetDefinition();
  if (particleDef == G4Electron::Definition()) {
    fParticleStats.numElectrons++;
  } else if (particleDef == G4Positron::Definition()) {
    fParticleStats.numPositrons++;
  } else if (particleDef == G4Gamma::Definition()) {
    fParticleStats.numGammas++;
  } else if (particleDef == G4Neutron::Definition()) {
    fParticleStats.numNeutrons++;
  } else {
    fParticleStats.numOthers++;
  }
}
