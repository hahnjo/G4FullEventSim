// SPDX-License-Identifier: Apache-2.0

#include "SteppingAction.hh"

#include "EventAction.hh"
#include "PhysicsSettings.hh"

#include <G4LogicalVolume.hh>
#include <G4Region.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VPhysicalVolume.hh>

SteppingAction::SteppingAction(const PhysicsSettings &physicsSettings,
                               EventAction &eventAction)
    : fPhysicsSettings(physicsSettings), fEventAction(eventAction) {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
  fEventAction.AccountStep(step);

  G4Track *track = step->GetTrack();
  if (track->GetTrackStatus() != fAlive) {
    return;
  }

  double maxTrackTime = fPhysicsSettings.maxTrackTime;
  if (maxTrackTime > 0 && track->GetGlobalTime() > maxTrackTime) {
    track->SetTrackStatus(fStopAndKill);
    return;
  }

  const G4LogicalVolume *volume = track->GetVolume()->GetLogicalVolume();
  const G4Region *currentRegion = volume->GetRegion();
  auto &deadRegions = fPhysicsSettings.deadRegions;
  if (deadRegions.find(currentRegion) != deadRegions.end()) {
    track->SetTrackStatus(fStopAndKill);
    return;
  }
}
