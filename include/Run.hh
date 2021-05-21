// SPDX-License-Identifier: Apache-2.0

#ifndef RUN_HH
#define RUN_HH

#include "ParticleStatistics.hh"

#include <G4Run.hh>

class Run final : public G4Run {
public:
  const ParticleStatistics &GetStatistics() const { return fStatistics; }

  void Merge(const G4Run *aRun) override {
    G4Run::Merge(aRun);
    AddStatistics(static_cast<const Run *>(aRun)->fStatistics);
  }
  void AddStatistics(const ParticleStatistics &stats) { fStatistics += stats; }

private:
  ParticleStatistics fStatistics;
};

#endif // RUN_HH
