// SPDX-License-Identifier: Apache-2.0

#include "RunAction.hh"

#include "ParticleStatistics.hh"
#include "Run.hh"

G4Run *RunAction::GenerateRun() { return new Run; }

void MasterRunAction::BeginOfRunAction(const G4Run *) { fTimer.Start(); }

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
