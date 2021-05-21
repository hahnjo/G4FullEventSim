// SPDX-License-Identifier: Apache-2.0

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PhysicsSettings.hh"

#include <G4RunManager.hh>
#include <G4RunManagerFactory.hh>
#include <G4UImanager.hh>

#include <memory>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    G4cerr << "Usage: " << argv[0] << " <g4.mac>" << G4endl;
    return 1;
  }

  std::unique_ptr<G4RunManager> runManager(
      G4RunManagerFactory::CreateRunManager());

  std::unique_ptr<PhysicsSettingsMessenger> psMessenger(
      new PhysicsSettingsMessenger);
  const PhysicsSettings &physicsSettings = psMessenger->GetPhysicsSettings();

  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new PhysicsList(physicsSettings));
  runManager->SetUserInitialization(new ActionInitialization(physicsSettings));

  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  G4String command = "/control/execute ";
  UImanager->ApplyCommand(command + argv[1]);

  return 0;
}
