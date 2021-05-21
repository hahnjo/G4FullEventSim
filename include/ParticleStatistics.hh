// SPDX-License-Identifier: Apache-2.0

#ifndef PARTICLE_STATISTICS_HH
#define PARTICLE_STATISTICS_HH

struct ParticleStatistics final {
  unsigned long long numPrimaries = 0;
  unsigned long long numSecondaries = 0;
  unsigned long long numElectrons = 0;
  unsigned long long numPositrons = 0;
  unsigned long long numGammas = 0;
  unsigned long long numNeutrons = 0;
  unsigned long long numOthers = 0;

  void Clear() {
    numPrimaries = 0;
    numSecondaries = 0;
    numElectrons = 0;
    numPositrons = 0;
    numGammas = 0;
    numNeutrons = 0;
    numOthers = 0;
  }

  void operator+=(const ParticleStatistics &stats) {
    numPrimaries += stats.numPrimaries;
    numSecondaries += stats.numSecondaries;
    numElectrons += stats.numElectrons;
    numPositrons += stats.numPositrons;
    numGammas += stats.numGammas;
    numNeutrons += stats.numNeutrons;
    numOthers += stats.numOthers;
  }
};

#endif // PARTICLE_STATISTICS_HH
