// SPDX-License-Identifier: Apache-2.0

#include "ActionInitialization.hh"

#include "Actions.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

#include <G4RunManager.hh>

ActionInitialization::ActionInitialization(
    const PhysicsSettings &physicsSettings)
    : fPhysicsSettings(physicsSettings) {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new MasterRunAction);
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

  if (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM) {
    SetUserAction(new MasterRunAction);
  } else {
    SetUserAction(new RunAction);
  }

  EventAction *eventAction = new EventAction;
  SetUserAction(eventAction);
  SetUserAction(new TrackingAction(eventAction));
  SetUserAction(new SteppingAction(fPhysicsSettings));
}
