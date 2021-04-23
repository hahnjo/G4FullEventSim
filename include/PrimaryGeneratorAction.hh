// SPDX-License-Identifier: Apache-2.0

#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>

class PrimaryGeneratorAction final : public G4VUserPrimaryGeneratorAction {
public:
  void GeneratePrimaries(G4Event *anEvent) override;
};

#endif // PRIMARY_GENERATOR_ACTION_HH
