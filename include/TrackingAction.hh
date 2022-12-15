// SPDX-License-Identifier: Apache-2.0

#ifndef TRACKING_ACTION_HH
#define TRACKING_ACTION_HH

#include <G4UserTrackingAction.hh>

class EventAction;
class G4Track;

class TrackingAction final : public G4UserTrackingAction {
public:
  TrackingAction(EventAction &eventAction);

  void PreUserTrackingAction(const G4Track *track) override;

  const G4Track *GetTrack() const { return fTrack; }

private:
  EventAction &fEventAction;

  const G4Track *fTrack;
};

#endif // TRACKING_ACTION_HH
