// SPDX-License-Identifier: Apache-2.0

#include "PhysicsList.hh"

#include "EMshowerProcess.hh"

#include <G4Electron.hh>
#include <G4Gamma.hh>
#include <G4Positron.hh>
#include <G4ProcessManager.hh>

void PhysicsList::ConstructProcess() {
  Base::ConstructProcess();

  auto *process = new EMshowerProcess;
  G4Electron::Definition()->GetProcessManager()->AddDiscreteProcess(process);
  G4Positron::Definition()->GetProcessManager()->AddDiscreteProcess(process);
  G4Gamma::Definition()->GetProcessManager()->AddDiscreteProcess(process);
}
