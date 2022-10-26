// SPDX-License-Identifier: Apache-2.0

#ifndef RUN_HH
#define RUN_HH

#include "ParticleStatistics.hh"
#include "StepStatistics.hh"

#include <G4Run.hh>

class Run final : public G4Run {
public:
  const ParticleStatistics &GetParticleStatistics() const {
    return fParticleStats;
  }
  const StepStatistics &GetStepStatistics() const { return fStepStats; }

  void Merge(const G4Run *aRun) override {
    G4Run::Merge(aRun);
    const auto *run = static_cast<const Run *>(aRun);
    AddStatistics(run->fParticleStats, run->fStepStats);
  }
  void AddStatistics(const ParticleStatistics &particleStats,
                     const StepStatistics &stepStats) {
    fParticleStats += particleStats;
    fStepStats += stepStats;
  }

private:
  ParticleStatistics fParticleStats;
  StepStatistics fStepStats;
};

#endif // RUN_HH
