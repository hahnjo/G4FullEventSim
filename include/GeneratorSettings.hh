// SPDX-License-Identifier: Apache-2.0

#ifndef GENERATOR_SETTINGS_HH
#define GENERATOR_SETTINGS_HH

#include <G4UImessenger.hh>

#include <memory>
#include <vector>

namespace HepMC3 {
class GenEvent;
};

struct GeneratorSettings final {
  std::vector<const HepMC3::GenEvent *> events;
};

class G4UIcmdWithAString;
class G4UIdirectory;

class GeneratorSettingsMessenger final : public G4UImessenger {
public:
  GeneratorSettingsMessenger();
  ~GeneratorSettingsMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

  const GeneratorSettings &GetGeneratorSettings() const { return fSettings; }

private:
  GeneratorSettings fSettings;

  std::unique_ptr<G4UIdirectory> fDirectory;
  std::unique_ptr<G4UIcmdWithAString> fHepMCfile;
};

#endif // GENERATOR_SETTINGS_HH
