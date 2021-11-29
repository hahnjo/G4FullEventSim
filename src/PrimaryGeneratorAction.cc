// SPDX-License-Identifier: Apache-2.0

#include "PrimaryGeneratorAction.hh"

#include "config.hh"

#include "GeneratorSettings.hh"

#include <G4Event.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>

#if HAVE_HEPMC3
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>

#include <HepMC3/FourVector.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/Units.h>
#endif

PrimaryGeneratorAction::PrimaryGeneratorAction(
    const GeneratorSettings &generatorSettings)
    : fGeneratorSettings(generatorSettings) {}

#if HAVE_HEPMC3
static double toG4length(double l, HepMC3::Units::LengthUnit u) {
  HepMC3::Units::convert(l, u, HepMC3::Units::MM);
  return l * mm;
}

static double toG4momentum(double p, HepMC3::Units::MomentumUnit u) {
  HepMC3::Units::convert(p, u, HepMC3::Units::MEV);
  return p * MeV;
}
#endif

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
#if HAVE_HEPMC3
  if (!fGeneratorSettings.events.empty()) {
    size_t eventIdx = anEvent->GetEventID() % fGeneratorSettings.events.size();
    const HepMC3::GenEvent *event = fGeneratorSettings.events[eventIdx];

    auto momentumUnit = event->momentum_unit();
    auto lengthUnit = event->length_unit();
    for (HepMC3::ConstGenVertexPtr vertex : event->vertices()) {
      bool empty = true;
      // Check if there is a primary particle in this vertex.
      for (HepMC3::ConstGenParticlePtr particle : vertex->particles_out()) {
        if (particle->status() == 1) {
          empty = false;
          break;
        }
      }
      if (empty) {
        continue;
      }

      // Create the vertex to hold the primary particles.
      const HepMC3::FourVector &pos = vertex->position();
      double x = toG4length(pos.x(), lengthUnit);
      double y = toG4length(pos.y(), lengthUnit);
      double z = toG4length(pos.z(), lengthUnit);
      double t = toG4length(pos.t(), lengthUnit) / c_light;

      G4PrimaryVertex *g4Vertex = new G4PrimaryVertex(x, y, z, t);

      for (HepMC3::ConstGenParticlePtr particle : vertex->particles_out()) {
        if (particle->status() != 1) {
          continue;
        }

        // Add the primary particle to the vertex.
        int pid = particle->pid();
        const HepMC3::FourVector &mom = particle->momentum();
        double px = toG4momentum(mom.px(), momentumUnit);
        double py = toG4momentum(mom.py(), momentumUnit);
        double pz = toG4momentum(mom.pz(), momentumUnit);
        double e = toG4momentum(mom.e(), momentumUnit);

        G4PrimaryParticle *g4Particle =
            new G4PrimaryParticle(pid, px, py, pz, e);
        g4Vertex->SetPrimary(g4Particle);
      }
      anEvent->AddPrimaryVertex(g4Vertex);
    }
    return;
  }
#endif

  fParticleSource.GeneratePrimaryVertex(anEvent);
}
