// SPDX-License-Identifier: Apache-2.0

#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4GeneralParticleSource.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

#include <memory>

class GeneratorSettings;

class PrimaryGeneratorAction final : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction(const GeneratorSettings &generatorSettings);

  void GeneratePrimaries(G4Event *anEvent) override;

private:
  const GeneratorSettings &fGeneratorSettings;
  G4GeneralParticleSource fParticleSource;
};

#endif // PRIMARY_GENERATOR_ACTION_HH
