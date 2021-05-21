// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_SETTINGS_HH
#define PHYSICS_SETTINGS_HH

#include <G4UImessenger.hh>

struct RouletteParameters final {
  double prob = 1.0;
  double limit = 0.0;
};

struct PhysicsSettings final {
  double maxTrackTime = -1;

  RouletteParameters rrGamma;
  RouletteParameters rrNeutron;
};

class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;

class PhysicsSettingsMessenger final : public G4UImessenger {
public:
  PhysicsSettingsMessenger();
  ~PhysicsSettingsMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

  const PhysicsSettings &GetPhysicsSettings() const { return fSettings; }

private:
  PhysicsSettings fSettings;

  std::unique_ptr<G4UIcmdWithADoubleAndUnit> fMaxTrackTime;
  std::unique_ptr<G4UIdirectory> fRussianRoulette;
  std::unique_ptr<G4UIcommand> fRRgamma;
  std::unique_ptr<G4UIcommand> fRRneutron;
};

#endif // PHYSICS_SETTINGS_HH
