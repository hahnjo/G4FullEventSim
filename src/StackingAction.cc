// SPDX-License-Identifier: Apache-2.0

#include "StackingAction.hh"

#include "PhysicsSettings.hh"
#include "TrackingAction.hh"

#include <G4Electron.hh>
#include <G4Gamma.hh>
#include <G4Neutron.hh>
#include <G4Positron.hh>
#include <G4Track.hh>
#include <Randomize.hh>

class G4ParticleDefinition;

StackingAction::StackingAction(const PhysicsSettings &physicsSettings,
                               const TrackingAction &trackingAction)
    : fPhysicsSettings(physicsSettings), fTrackingAction(trackingAction) {}

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track *aTrack) {
  if (aTrack->GetParentID() == 0) {
    // Do not kill primaries.
    return fUrgent;
  }

  const G4ParticleDefinition *particleDef = aTrack->GetDefinition();

  // Russian roulette
  double currentWeight = aTrack->GetWeight();
  if (currentWeight <= 1.0) {
    const RouletteParameters *params = nullptr;

    if (particleDef == G4Gamma::Definition()) {
      // For gammas, check that their mother is no other EM particle.
      const G4Track *mother = fTrackingAction.GetTrack();
      const G4ParticleDefinition *motherDef = mother->GetDefinition();
      if (motherDef != G4Electron::Definition() &&
          motherDef != G4Positron::Definition() &&
          motherDef != G4Gamma::Definition()) {
        params = &fPhysicsSettings.rrGamma;
      }
    } else if (particleDef == G4Neutron::Definition()) {
      params = &fPhysicsSettings.rrNeutron;
    }

    double energy = aTrack->GetKineticEnergy();
    if (params != nullptr && params->prob < 1.0 && energy < params->limit) {
      if (G4UniformRand() < params->prob) {
        double newWeight = currentWeight / params->prob;
        const_cast<G4Track *>(aTrack)->SetWeight(newWeight);
      } else {
        return fKill;
      }
    }
  }

  return fUrgent;
}
