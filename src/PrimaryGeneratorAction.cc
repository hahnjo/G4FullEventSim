// SPDX-License-Identifier: Apache-2.0

#include "PrimaryGeneratorAction.hh"

#include "config.hh"

#include <G4Event.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

#if HAVE_HEPMC3
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>

#include <HepMC3/FourVector.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/ReaderAscii.h>
#include <HepMC3/Units.h>
#endif

#include <memory>

class PrimaryGeneratorMessenger final : public G4UImessenger {
public:
  PrimaryGeneratorMessenger(PrimaryGeneratorAction *generatorAction);

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  PrimaryGeneratorAction *fGeneratorAction;

  std::unique_ptr<G4UIdirectory> fDirectory;
  std::unique_ptr<G4UIcmdWithAString> fHepMCfile;
};

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
    PrimaryGeneratorAction *generatorAction)
    : fGeneratorAction(generatorAction) {
  fDirectory.reset(new G4UIdirectory("/generator/"));
  fDirectory->SetGuidance("Set generator parameters.");

#if HAVE_HEPMC3
  fHepMCfile.reset(new G4UIcmdWithAString("/generator/hepMCfile", this));
  fHepMCfile->SetGuidance("Read event from HepMC3 file.");
#endif
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command,
                                            G4String newValue) {
#if HAVE_HEPMC3
  if (command == fHepMCfile.get()) {
    // Read one event.
    HepMC3::ReaderAscii reader(newValue);
    std::unique_ptr<HepMC3::GenEvent> event(new HepMC3::GenEvent);
    if (!reader.read_event(*event)) {
      G4Exception("PrimaryGeneratorMessenger::SetNewValue", "0001",
                  FatalException, "Could not load HepMC3 file!");
    }
    fGeneratorAction->fEvent = event.release();
  }
#endif
}

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  fMessenger.reset(new PrimaryGeneratorMessenger(this));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
#if HAVE_HEPMC3
  delete fEvent;
#endif
}

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
  if (fEvent != nullptr) {
    auto momentumUnit = fEvent->momentum_unit();
    auto lengthUnit = fEvent->length_unit();
    for (HepMC3::GenVertexPtr vertex : fEvent->vertices()) {
      bool empty = true;
      // Check if there is a primary particle in this vertex.
      for (HepMC3::GenParticlePtr particle : vertex->particles_out()) {
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

      for (HepMC3::GenParticlePtr particle : vertex->particles_out()) {
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
