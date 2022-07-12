// SPDX-License-Identifier: Apache-2.0

#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

#include <G4RunManager.hh>

ActionInitialization::ActionInitialization(
    const GeneratorSettings &generatorSettings,
    const PhysicsSettings &physicsSettings)
    : fGeneratorSettings(generatorSettings), fPhysicsSettings(physicsSettings) {
}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new MasterRunAction(fGeneratorSettings, fPhysicsSettings));
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction(fGeneratorSettings));

  if (G4RunManager::GetRunManager()->GetRunManagerType() ==
      G4RunManager::sequentialRM) {
    SetUserAction(new MasterRunAction(fGeneratorSettings, fPhysicsSettings));
  } else {
    SetUserAction(new RunAction);
  }

  EventAction *eventAction = new EventAction;
  SetUserAction(eventAction);
  TrackingAction *trackingAction = new TrackingAction(*eventAction);
  SetUserAction(trackingAction);
  SetUserAction(new SteppingAction(fPhysicsSettings));

  SetUserAction(new StackingAction(fPhysicsSettings, *trackingAction));
}
