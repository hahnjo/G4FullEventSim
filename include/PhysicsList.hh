// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <G4VUserPhysicsList.hh>

class PhysicsList final : public G4VUserPhysicsList {
public:
  void ConstructParticle() override;
  void ConstructProcess() override;
};

#endif // PHYSICS_LIST_HH
