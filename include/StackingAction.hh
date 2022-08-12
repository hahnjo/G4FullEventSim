// SPDX-License-Identifier: Apache-2.0

#ifndef STACKING_ACTION_HH
#define STACKING_ACTION_HH

#include <G4UserStackingAction.hh>

struct PhysicsSettings;
class TrackingAction;

class StackingAction final : public G4UserStackingAction {
  friend class StackingActionMessenger;

public:
  StackingAction(const PhysicsSettings &physicsSettings,
                 const TrackingAction &trackingAction);

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *aTrack) override;

private:
  const PhysicsSettings &fPhysicsSettings;
  const TrackingAction &fTrackingAction;
};

#endif // STACKING_ACTION_HH
