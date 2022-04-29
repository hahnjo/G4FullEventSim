// SPDX-License-Identifier: Apache-2.0

#include "TrackingAction.hh"

#include "EventAction.hh"

TrackingAction::TrackingAction(EventAction &eventAction)
    : fEventAction(eventAction) {}

void TrackingAction::PreUserTrackingAction(const G4Track *track) {
  fTrack = track;
  fEventAction.AccountTrack(track);
}
