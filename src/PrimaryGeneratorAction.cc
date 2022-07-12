// SPDX-License-Identifier: Apache-2.0

#include "PrimaryGeneratorAction.hh"

#include "config.hh"

#include "GeneratorSettings.hh"

#include <G4Event.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>

#if HAVE_HEPMC3
#include <G4LogicalVolume.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4TransportationManager.hh>
#include <G4VSolid.hh>

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

    auto *transMgr = G4TransportationManager::GetTransportationManager();
    auto *worldVol = transMgr->GetNavigatorForTracking()->GetWorldVolume();
    auto *worldSolid = worldVol->GetLogicalVolume()->GetSolid();

    auto momentumUnit = event->momentum_unit();
    auto lengthUnit = event->length_unit();
    for (HepMC3::ConstGenVertexPtr vertex : event->vertices()) {
      // Check if there are primary particles in this vertex. If so, sum their
      // energy to give a meaningful error message in case the vertex is outside
      // the world.
      bool empty = true;
      double totalE = 0;
      for (HepMC3::ConstGenParticlePtr particle : vertex->particles_out()) {
        if (particle->status() == 1) {
          // Check the particle's energy.
          double e = toG4momentum(particle->momentum().e(), momentumUnit);
          if (e < fGeneratorSettings.minEnergy) {
            continue;
          }

          empty = false;
          totalE += e;
        }
      }
      if (empty) {
        continue;
      }

      // Locate the vertex to hold the primary particles.
      const HepMC3::FourVector &pos = vertex->position();
      double x = toG4length(pos.x(), lengthUnit);
      double y = toG4length(pos.y(), lengthUnit);
      double z = toG4length(pos.z(), lengthUnit);
      double t = toG4length(pos.t(), lengthUnit) / c_light;

      // Check if the vertex is inside the world.
      G4ThreeVector g4Pos{x, y, z};
      if (worldSolid->Inside(g4Pos) != kInside) {
        G4ExceptionDescription msg;
        msg << "Vertex " << vertex->id() << " of event "
            << event->event_number() << " at position (" << x << ", " << y
            << ", " << z << ") is outside the current world volume!\n"
            << "Not generating the primaries originating from this vertex"
            << " with a total of " << totalE / MeV << " MeV.";
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries", "0002",
                    JustWarning, msg);
        continue;
      }

      // Check if the vertex is cut out.
      double eta = g4Pos.eta();
      if (eta < fGeneratorSettings.minEta || eta > fGeneratorSettings.maxEta) {
        continue;
      }

      // Create the vertex and add the primary particles.
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

        // Check if the particle has enough energy.
        if (e < fGeneratorSettings.minEnergy) {
          continue;
        }

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
