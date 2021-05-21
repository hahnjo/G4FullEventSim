// SPDX-License-Identifier: Apache-2.0

#include "SteppingAction.hh"

#include "PhysicsSettings.hh"

#include <G4Step.hh>
#include <G4Track.hh>

SteppingAction::SteppingAction(const PhysicsSettings &physicsSettings)
    : fPhysicsSettings(physicsSettings) {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
  G4Track *track = step->GetTrack();
  if (track->GetTrackStatus() != fAlive) {
    return;
  }

  double maxTrackTime = fPhysicsSettings.maxTrackTime;
  if (maxTrackTime > 0 && track->GetGlobalTime() > maxTrackTime) {
    track->SetTrackStatus(fStopAndKill);
    return;
  }
}
