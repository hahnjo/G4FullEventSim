// SPDX-License-Identifier: Apache-2.0

#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

struct GeneratorSettings;
struct OutputSettings;
struct PhysicsSettings;

class ActionInitialization final : public G4VUserActionInitialization {
public:
  ActionInitialization(const GeneratorSettings &generatorSettings,
                       const PhysicsSettings &physicsSettings,
                       const OutputSettings &outputSettings);

  void BuildForMaster() const override;
  void Build() const override;

private:
  const GeneratorSettings &fGeneratorSettings;
  const PhysicsSettings &fPhysicsSettings;
  const OutputSettings &fOutputSettings;
};

#endif // ACTION_INITIALIZATION_HH
