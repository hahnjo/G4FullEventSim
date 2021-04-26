// SPDX-License-Identifier: Apache-2.0

#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4GeneralParticleSource.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

#include <memory>

class PrimaryGeneratorMessenger;

namespace HepMC3 {
class GenEvent;
};

class PrimaryGeneratorAction final : public G4VUserPrimaryGeneratorAction {
  friend class PrimaryGeneratorMessenger;

public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event *anEvent) override;

private:
  G4GeneralParticleSource fParticleSource;
  std::unique_ptr<PrimaryGeneratorMessenger> fMessenger;

  HepMC3::GenEvent *fEvent = nullptr;
};

#endif // PRIMARY_GENERATOR_ACTION_HH
