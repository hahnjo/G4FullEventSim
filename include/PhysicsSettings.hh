// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_SETTINGS_HH
#define PHYSICS_SETTINGS_HH

#include <G4UImessenger.hh>

struct PhysicsSettings final {
  double maxTrackTime = -1;
};

class G4UIcmdWithADoubleAndUnit;

class PhysicsSettingsMessenger final : public G4UImessenger {
public:
  PhysicsSettingsMessenger();
  ~PhysicsSettingsMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

  const PhysicsSettings &GetPhysicsSettings() const { return fSettings; }

private:
  PhysicsSettings fSettings;

  std::unique_ptr<G4UIcmdWithADoubleAndUnit> fMaxTrackTime;
};

#endif // PHYSICS_SETTINGS_HH
