// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <G4VModularPhysicsList.hh>

class PhysicsSettings;

class PhysicsList final : public G4VModularPhysicsList {
  using Base = G4VModularPhysicsList;

public:
  PhysicsList(const PhysicsSettings &physicsSettings);

  void ConstructProcess() override;
};

#endif // PHYSICS_LIST_HH
