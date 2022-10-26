// SPDX-License-Identifier: Apache-2.0

#include "EventAction.hh"

#include "OutputSettings.hh"
#include "ParticleStatistics.hh"
#include "Run.hh"

#include <G4DynamicParticle.hh>
#include <G4Electron.hh>
#include <G4EmProcessSubType.hh>
#include <G4Gamma.hh>
#include <G4Neutron.hh>
#include <G4Positron.hh>
#include <G4ProcessType.hh>
#include <G4RunManager.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

EventAction::EventAction(const OutputSettings &outputSettings)
    : fOutputSettings(outputSettings) {}

void EventAction::BeginOfEventAction(const G4Event *) {
  fParticleStats.Clear();
  fStepStats.Clear();
}

void EventAction::EndOfEventAction(const G4Event *) {
  auto *runManager = G4RunManager::GetRunManager();
  Run *run = static_cast<Run *>(runManager->GetNonConstCurrentRun());
  run->AddStatistics(fParticleStats, fStepStats);
}

void EventAction::AccountTrack(const G4Track *track) {
  if (fOutputSettings.printParticleStats) {
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
}

void EventAction::AccountStep(const G4Step *step) {
  if (fOutputSettings.printStepStats) {
    const G4Track *track = step->GetTrack();
    const G4DynamicParticle *particle = track->GetDynamicParticle();
    const G4ParticleDefinition *particleDef = track->GetDefinition();

    const G4VProcess *proc = step->GetPostStepPoint()->GetProcessDefinedStep();
    const G4String &processName = proc->GetProcessName();

    if (particle->GetCharge() != 0) {
      fStepStats.numChargedSteps++;
      if (particleDef == G4Electron::Definition()) {
        fStepStats.numElectronSteps++;
        fStepStats.numElectronStepsByProcess[processName]++;
      } else if (particleDef == G4Positron::Definition()) {
        fStepStats.numPositronSteps++;
        fStepStats.numPositronStepsByProcess[processName]++;
      }
    } else {
      fStepStats.numNeutralSteps++;
      if (particleDef == G4Gamma::Definition()) {
        fStepStats.numGammaSteps++;
        fStepStats.numGammaStepsByProcess[processName]++;
      }
    }
  }
}
