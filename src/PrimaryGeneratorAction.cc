// SPDX-License-Identifier: Apache-2.0

#include "PrimaryGeneratorAction.hh"

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fParticleSource.GeneratePrimaryVertex(anEvent);
}
