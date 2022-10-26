// SPDX-License-Identifier: Apache-2.0

#ifndef OUTPUT_SETTINGS_HH
#define OUTPUT_SETTINGS_HH

#include <G4UImessenger.hh>

struct OutputSettings final {
  bool printParticleStats = true;
  bool printStepStats = false;
};

class G4UIcmdWithABool;
class G4UIdirectory;

class OutputSettingsMessenger final : public G4UImessenger {
public:
  OutputSettingsMessenger();
  ~OutputSettingsMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

  const OutputSettings &GetOutputSettings() const { return fSettings; }

private:
  OutputSettings fSettings;

  std::unique_ptr<G4UIdirectory> fOutputDir;
  std::unique_ptr<G4UIcmdWithABool> fPrintParticleStats;
  std::unique_ptr<G4UIcmdWithABool> fPrintStepStats;
};

#endif
