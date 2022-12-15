// SPDX-License-Identifier: Apache-2.0

#include "RunAction.hh"

#include "GeneratorSettings.hh"
#include "OutputSettings.hh"
#include "ParticleStatistics.hh"
#include "PhysicsSettings.hh"
#include "Run.hh"
#include "StepStatistics.hh"

#include <G4Run.hh>
#include <G4SystemOfUnits.hh>
#include <G4ios.hh>

#include <algorithm>
#include <utility>
#include <vector>

G4Run *RunAction::GenerateRun() { return new Run; }

MasterRunAction::MasterRunAction(const GeneratorSettings &generatorSettings,
                                 const PhysicsSettings &physicsSettings,
                                 const OutputSettings &outputSettings)
    : fGeneratorSettings(generatorSettings), fPhysicsSettings(physicsSettings),
      fOutputSettings(outputSettings) {}

void MasterRunAction::BeginOfRunAction(const G4Run *) {
  G4cout << "Starting run; parameters:" << G4endl;

  G4cout << "  generator cuts:" << G4endl;
  G4cout << "    eta   : ";
  double minEta = fGeneratorSettings.minEta;
  double maxEta = fGeneratorSettings.maxEta;
  if (minEta > -DBL_MAX || maxEta < DBL_MAX) {
    G4cout << "min = " << minEta << ", max = " << maxEta;
  } else {
    G4cout << "(disabled)";
  }
  G4cout << G4endl;

  G4cout << "    energy: ";
  double minEnergy = fGeneratorSettings.minEnergy;
  if (minEnergy > 0) {
    G4cout << "min = " << minEnergy / MeV << " MeV";
  } else {
    G4cout << "(disabled)";
  }
  G4cout << G4endl;

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

  const Run *run = static_cast<const Run *>(aRun);

  if (fOutputSettings.printParticleStats) {
    G4cout << G4endl;

    const ParticleStatistics &particleStats = run->GetParticleStatistics();
    double numPrimaries = ((double)particleStats.numPrimaries) / events;
    double numSecondaries = ((double)particleStats.numSecondaries) / events;
    double numElectrons = ((double)particleStats.numElectrons) / events;
    double numPositrons = ((double)particleStats.numPositrons) / events;
    double numGammas = ((double)particleStats.numGammas) / events;
    double numNeutrons = ((double)particleStats.numNeutrons) / events;
    double numOthers = ((double)particleStats.numOthers) / events;

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

  if (fOutputSettings.printStepStats) {
    G4cout << G4endl;

    auto printStepsByProcess =
        [&](const StepStatistics::StepsByProcessMap &map) {
          // Get all entries and sort them in descending order.
          using K = StepStatistics::StepsByProcessKey;
          using V = StepStatistics::StepsByProcessValue;
          using P = std::pair<K, V>;
          std::vector<P> entries(map.begin(), map.end());
          std::sort(entries.begin(), entries.end(),
                    [](const P &a, const P &b) { return a.second > b.second; });

          for (const auto &stepsByProc : entries) {
            double average = ((double)stepsByProc.second) / events;
            G4cout << "      " << stepsByProc.first << ": " << average
                   << G4endl;
          }
        };

    const StepStatistics &stepStats = run->GetStepStatistics();
    double numChargedSteps = ((double)stepStats.numChargedSteps) / events;
    double numElectronSteps = ((double)stepStats.numElectronSteps) / events;
    double numPositronSteps = ((double)stepStats.numPositronSteps) / events;
    double numNeutralSteps = ((double)stepStats.numNeutralSteps) / events;
    double numGammaSteps = ((double)stepStats.numGammaSteps) / events;

    G4cout << "Average number of steps per event:" << G4endl;
    G4cout << "  charged: " << numChargedSteps << G4endl;
    G4cout << "    electrons: " << numElectronSteps << G4endl;
    printStepsByProcess(stepStats.numElectronStepsByProcess);
    G4cout << "    positrons: " << numPositronSteps << G4endl;
    printStepsByProcess(stepStats.numPositronStepsByProcess);
    G4cout << "  neutral: " << numNeutralSteps << G4endl;
    G4cout << "    gammas: " << numGammaSteps << G4endl;
    printStepsByProcess(stepStats.numGammaStepsByProcess);
  }
}
