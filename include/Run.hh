// SPDX-License-Identifier: Apache-2.0

#ifndef RUN_HH
#define RUN_HH

#include "ParticleStatistics.hh"

#include <G4Run.hh>

class Run final : public G4Run {
public:
  const ParticleStatistics &GetParticleStatistics() const {
    return fParticleStats;
  }

  void Merge(const G4Run *aRun) override {
    G4Run::Merge(aRun);
    AddStatistics(static_cast<const Run *>(aRun)->fParticleStats);
  }
  void AddStatistics(const ParticleStatistics &particleStats) {
    fParticleStats += particleStats;
  }

private:
  ParticleStatistics fParticleStats;
};

#endif // RUN_HH
