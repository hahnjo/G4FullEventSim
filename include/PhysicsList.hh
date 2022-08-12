// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <G4VModularPhysicsList.hh>

struct PhysicsSettings;

class PhysicsList final : public G4VModularPhysicsList {
public:
  PhysicsList(const PhysicsSettings &physicsSettings);
};

#endif // PHYSICS_LIST_HH
