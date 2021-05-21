// SPDX-License-Identifier: Apache-2.0

#include "RunAction.hh"

#include "ParticleStatistics.hh"
#include "PhysicsSettings.hh"
#include "Run.hh"

#include <G4SystemOfUnits.hh>

G4Run *RunAction::GenerateRun() { return new Run; }

MasterRunAction::MasterRunAction(const PhysicsSettings &physicsSettings)
    : fPhysicsSettings(physicsSettings) {}

void MasterRunAction::BeginOfRunAction(const G4Run *) {
  G4cout << "Starting run; parameters:" << G4endl;

  G4cout << "  maximum track time: ";
  double maxTrackTime = fPhysicsSettings.maxTrackTime;
  if (maxTrackTime > 0) {
    G4cout << maxTrackTime / ns << " ns";
  } else {
    G4cout << "(disabled)";
  }
  G4cout << G4endl;

  G4cout << "  Russian Roulette:" << G4endl;
  auto printRR = [](const char *particle, const RouletteParameters *params) {
    G4cout << "    " << particle << ": ";
    if (params->prob < 1.0 && params->limit > 0) {
      G4cout << "prob = " << params->prob << ", ";
      G4cout << "limit = " << params->limit / MeV << " MeV";
    } else {
      G4cout << "(disabled)";
    }
    G4cout << G4endl;
  };
  printRR("gamma  ", &fPhysicsSettings.rrGamma);
  printRR("neutron", &fPhysicsSettings.rrNeutron);
  G4cout << G4endl;

  fTimer.Start();
}

void MasterRunAction::EndOfRunAction(const G4Run *aRun) {
  fTimer.Stop();

  // clang-format off
  G4int events = aRun->GetNumberOfEvent();
  G4cout << " ============================================================================== " << G4endl;
  G4cout << "  Run terminated, " << events << " events transported" << G4endl;
  G4cout << "  Time: " << fTimer << G4endl;
  G4cout << " ============================================================================== " << G4endl;
  // clang-format on
  G4cout << G4endl;

  const Run *run = static_cast<const Run *>(aRun);
  const ParticleStatistics &stats = run->GetStatistics();
  double numPrimaries = ((double)stats.numPrimaries) / events;
  double numSecondaries = ((double)stats.numSecondaries) / events;
  double numElectrons = ((double)stats.numElectrons) / events;
  double numPositrons = ((double)stats.numPositrons) / events;
  double numGammas = ((double)stats.numGammas) / events;
  double numNeutrons = ((double)stats.numNeutrons) / events;
  double numOthers = ((double)stats.numOthers) / events;

  G4cout << "Average number of particles per event:" << G4endl;
  G4cout << std::defaultfloat;
  G4cout << "  primaries:   " << numPrimaries << G4endl;
  G4cout << std::scientific;
  G4cout << "  secondaries: " << numSecondaries << G4endl;
  G4cout << "  tracked particles:" << G4endl;
  G4cout << "    electrons: " << numElectrons << G4endl;
  G4cout << "    positrons: " << numPositrons << G4endl;
  G4cout << "    gammas:    " << numGammas << G4endl;
  G4cout << "    neutrons:  " << numNeutrons << G4endl;
  G4cout << "    others:    " << numOthers << G4endl;
}
