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
    const PhysicsSettings &physicsSettings,
    const OutputSettings &outputSettings)
    : fGeneratorSettings(generatorSettings), fPhysicsSettings(physicsSettings),
      fOutputSettings(outputSettings) {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new MasterRunAction(fGeneratorSettings, fPhysicsSettings,
                                    fOutputSettings));
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction(fGeneratorSettings));

  if (G4RunManager::GetRunManager()->GetRunManagerType() ==
      G4RunManager::sequentialRM) {
    SetUserAction(new MasterRunAction(fGeneratorSettings, fPhysicsSettings,
                                      fOutputSettings));
  } else {
    SetUserAction(new RunAction);
  }

  EventAction *eventAction = new EventAction(fOutputSettings);
  SetUserAction(eventAction);
  TrackingAction *trackingAction = new TrackingAction(*eventAction);
  SetUserAction(trackingAction);
  SetUserAction(new SteppingAction(fPhysicsSettings, *eventAction));

  SetUserAction(new StackingAction(fPhysicsSettings, *trackingAction));
}
