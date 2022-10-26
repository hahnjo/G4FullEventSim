// SPDX-License-Identifier: Apache-2.0

#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include <G4UserSteppingAction.hh>

class EventAction;
struct PhysicsSettings;

class SteppingAction final : public G4UserSteppingAction {
public:
  SteppingAction(const PhysicsSettings &physicsSettings,
                 EventAction &eventAction);

  void UserSteppingAction(const G4Step *) override;

private:
  const PhysicsSettings &fPhysicsSettings;
  EventAction &fEventAction;
};

#endif // STEPPING_ACTION_HH
