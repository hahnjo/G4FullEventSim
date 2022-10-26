// SPDX-License-Identifier: Apache-2.0

#ifndef STEP_STATISTICS_HH
#define STEP_STATISTICS_HH

#include <string>
#include <unordered_map>

struct StepStatistics final {
  using StepsByProcessKey = std::string;
  using StepsByProcessValue = unsigned long long;
  using StepsByProcessMap =
      std::unordered_map<StepsByProcessKey, StepsByProcessValue>;

  unsigned long long numChargedSteps = 0;
  unsigned long long numElectronSteps = 0;
  StepsByProcessMap numElectronStepsByProcess;
  unsigned long long numPositronSteps = 0;
  StepsByProcessMap numPositronStepsByProcess;

  unsigned long long numNeutralSteps = 0;
  unsigned long long numGammaSteps = 0;
  StepsByProcessMap numGammaStepsByProcess;

  void Clear() {
    numChargedSteps = 0;
    numElectronSteps = 0;
    numElectronStepsByProcess.clear();
    numPositronSteps = 0;
    numPositronStepsByProcess.clear();

    numNeutralSteps = 0;
    numGammaSteps = 0;
    numGammaStepsByProcess.clear();
  }

  void operator+=(const StepStatistics &stats) {
    numChargedSteps += stats.numChargedSteps;
    numElectronSteps += stats.numElectronSteps;
    for (const auto &stepsByProc : stats.numElectronStepsByProcess) {
      numElectronStepsByProcess[stepsByProc.first] += stepsByProc.second;
    }
    numPositronSteps += stats.numPositronSteps;
    for (const auto &stepsByProc : stats.numPositronStepsByProcess) {
      numPositronStepsByProcess[stepsByProc.first] += stepsByProc.second;
    }

    numNeutralSteps += stats.numNeutralSteps;
    numGammaSteps += stats.numGammaSteps;
    for (const auto &stepsByProc : stats.numGammaStepsByProcess) {
      numGammaStepsByProcess[stepsByProc.first] += stepsByProc.second;
    }
  }
};

#endif // STEP_STATISTICS_HH
