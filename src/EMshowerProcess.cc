// SPDX-License-Identifier: Apache-2.0

#include "EMshowerProcess.hh"

#include "config.hh"

#include <G4EventManager.hh>
#include <G4LogicalVolume.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4StackManager.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>
#include <G4VPhysicalVolume.hh>

#if HAVE_HEPMC3
#include <G4SystemOfUnits.hh>

#include <HepMC3/FourVector.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/Units.h>
#include <HepMC3/WriterAscii.h>
#endif

class EMshowerProcessMessenger final : public G4UImessenger {
public:
  EMshowerProcessMessenger(EMshowerProcess *process);

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  EMshowerProcess *fProcess;

  std::unique_ptr<G4UIdirectory> fDirectory;
  std::unique_ptr<G4UIcmdWithAString> fRegion;
  std::unique_ptr<G4UIcmdWithADoubleAndUnit> fThreshold;
  std::unique_ptr<G4UIcmdWithABool> fStop;
  std::unique_ptr<G4UIcmdWithAString> fDump;
};

EMshowerProcessMessenger::EMshowerProcessMessenger(EMshowerProcess *process)
    : fProcess(process) {
  fDirectory.reset(new G4UIdirectory("/process/emShower/"));
  fDirectory->SetGuidance("Commands for the EM shower process.");

  fRegion.reset(new G4UIcmdWithAString("/process/emShower/region", this));
  fRegion->SetGuidance("Set the region where this process is active.");
  fRegion->SetParameterName("region", false);

  fThreshold.reset(
      new G4UIcmdWithADoubleAndUnit("/process/emShower/energyThreshold", this));
  fThreshold->SetGuidance(
      "Set the energy threshold above which this process is active.");
  fThreshold->SetParameterName("Emin", false);
  fThreshold->SetDefaultUnit("GeV");
  fThreshold->SetUnitCategory("Energy");

  fStop.reset(new G4UIcmdWithABool("/process/emShower/stopParticles", this));
  fStop->SetGuidance("Stop all EM particles entering the calorimeter.");
  fStop->SetDefaultValue(true);
  fStop->GetParameter(0)->SetOmittable(true);

#if HAVE_HEPMC3
  fDump.reset(new G4UIcmdWithAString("/process/emShower/dumpParticles", this));
  fDump->SetGuidance("Dump all stopped EM particles into a HepMC3 file.");
  fDump->SetParameterName("file", false);
#endif
}

void EMshowerProcessMessenger::SetNewValue(G4UIcommand *command,
                                           G4String newValue) {
  if (command == fRegion.get()) {
    const G4Region *region =
        G4RegionStore::GetInstance()->GetRegion(newValue, /*verbose=*/false);
    if (region == nullptr) {
      G4Exception("EMshowerProcessMessenger::SetNewValue", "0002",
                  FatalException, "Could not find region!");
    }
    fProcess->fRegion = region;
  } else if (command == fThreshold.get()) {
    double threshold = fThreshold->GetNewDoubleValue(newValue);
    fProcess->fEnergyThreshold = threshold;
  } else if (command == fStop.get()) {
    bool stop = fStop->GetNewBoolValue(newValue);
    fProcess->fStopParticles = stop;
  }
#if HAVE_HEPMC3
  else if (command == fDump.get()) {
    fProcess->fDumpParticles = newValue;
  }
#endif
}

EMshowerProcess::EMshowerProcess(const G4String &aName)
    : G4VDiscreteProcess(aName) {
  fMessenger.reset(new EMshowerProcessMessenger(this));
}

G4double EMshowerProcess::PostStepGetPhysicalInteractionLength(
    const G4Track &track, G4double previousStepSize,
    G4ForceCondition *condition) {
  return GetMeanFreePath(track, previousStepSize, condition);
}

G4double EMshowerProcess::GetMeanFreePath(const G4Track &track, G4double,
                                          G4ForceCondition *condition) {
  if (fStopParticles && track.GetKineticEnergy() > fEnergyThreshold) {
    const G4LogicalVolume *volume = track.GetVolume()->GetLogicalVolume();
    const G4Region *currentRegion = volume->GetRegion();
    if (fRegion == nullptr || currentRegion == fRegion) {
      *condition = Forced;
      return 0;
    }
  }
  *condition = NotForced;
  return DBL_MAX;
}

G4VParticleChange *EMshowerProcess::PostStepDoIt(const G4Track &track,
                                                 const G4Step &) {
  if (fStopParticles) {
    // Save the particle.
    const G4ThreeVector &pos = track.GetPosition();
    G4ThreeVector mom = track.GetMomentum();
    G4double e = track.GetTotalEnergy();
    const G4ParticleDefinition *particleDef = track.GetParticleDefinition();
    G4double mass = particleDef->GetPDGMass();
    G4int id = particleDef->GetPDGEncoding();
    fSavedTracks.emplace_back(pos.x(), pos.y(), pos.z(), mom.x(), mom.y(),
                              mom.z(), e, mass, id);

    fParticleChange.ProposeTrackStatus(fStopAndKill);
    return &fParticleChange;
  }
  return nullptr;
}

void EMshowerProcess::DumpParticles() const {
  if (!fStopParticles) {
    G4cerr << "Did not stop particles, not dumping anything." << G4endl;
    return;
  }

#if HAVE_HEPMC3
  HepMC3::WriterAscii writer(fDumpParticles);
  HepMC3::GenEvent event(HepMC3::Units::GEV, HepMC3::Units::MM);

  for (auto &&track : fSavedTracks) {
    HepMC3::FourVector pos(track.x / mm, track.y / mm, track.z / mm, /*t=*/0);
    auto vertex = std::make_shared<HepMC3::GenVertex>(pos);

    // Create a dummy particle that goes into the vertex...
    auto dummy = std::make_shared<HepMC3::GenParticle>();
    vertex->add_particle_in(dummy);

    HepMC3::FourVector momentum(track.px / GeV, track.py / GeV, track.pz / GeV,
                                track.e / GeV);
    auto particle = std::make_shared<HepMC3::GenParticle>(momentum, track.pdg,
                                                          /*status=*/1);
    particle->set_generated_mass(track.m / GeV);

    vertex->add_particle_out(particle);
    event.add_vertex(vertex);
  }

  writer.write_event(event);
#endif
}

void EMshowerProcess::EndTracking() {
  G4StackManager *stackManager =
      G4EventManager::GetEventManager()->GetStackManager();
  // FIXME: This is not correct, there may be particles in the waiting queues.
  if (stackManager->GetNUrgentTrack() == 0) {
    if (!fDumpParticles.empty()) {
      DumpParticles();
    }
    fSavedTracks.clear();
  }
}
