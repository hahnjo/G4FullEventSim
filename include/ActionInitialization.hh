// SPDX-License-Identifier: Apache-2.0

#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

class PhysicsSettings;

class ActionInitialization final : public G4VUserActionInitialization {
public:
  ActionInitialization(const PhysicsSettings &physicsSettings);

  void BuildForMaster() const override;
  void Build() const override;

private:
  const PhysicsSettings &fPhysicsSettings;
};

#endif // ACTION_INITIALIZATION_HH
