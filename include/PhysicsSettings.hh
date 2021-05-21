// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_SETTINGS_HH
#define PHYSICS_SETTINGS_HH

#include <G4SystemOfUnits.hh>
#include <G4UImessenger.hh>

struct RouletteParameters final {
  double prob = 1.0;
  double limit = 0.0;
};

struct FTFP_BERTparameters final {
  double minFTFP = 3.0 * GeV;
  double maxBERT = 6.0 * GeV;
};

struct PhysicsSettings final {
  double maxTrackTime = -1;

  RouletteParameters rrGamma;
  RouletteParameters rrNeutron;

  FTFP_BERTparameters FTFP_BERTpion;
  FTFP_BERTparameters FTFP_BERTkaon;
  FTFP_BERTparameters FTFP_BERTproton;
  FTFP_BERTparameters FTFP_BERTneutron;
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

  std::unique_ptr<G4UIdirectory> fFTFP_BERT;
  std::unique_ptr<G4UIcommand> fFTFP_BERTpion;
  std::unique_ptr<G4UIcommand> fFTFP_BERTkaon;
  std::unique_ptr<G4UIcommand> fFTFP_BERTproton;
  std::unique_ptr<G4UIcommand> fFTFP_BERTneutron;
};

#endif // PHYSICS_SETTINGS_HH
