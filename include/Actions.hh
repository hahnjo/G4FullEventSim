// SPDX-License-Identifier: Apache-2.0

#ifndef ACTIONS_HH
#define ACTIONS_HH

#include <G4Run.hh>
#include <G4Timer.hh>
#include <G4UserEventAction.hh>
#include <G4UserRunAction.hh>
#include <G4UserTrackingAction.hh>

struct ParticleStatistics {
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

class RunAction : public G4UserRunAction {
public:
  G4Run *GenerateRun() override { return new Run; }
};

class MasterRunAction final : public RunAction {
public:
  void BeginOfRunAction(const G4Run *) override;
  void EndOfRunAction(const G4Run *aRun) override;

private:
  G4Timer fTimer;
};

class EventAction final : public G4UserEventAction {
public:
  void BeginOfEventAction(const G4Event *) override;
  void EndOfEventAction(const G4Event *) override;

  void AccountTrack(const G4Track *track);

private:
  ParticleStatistics fStatistics;
};

class TrackingAction final : public G4UserTrackingAction {
public:
  TrackingAction(EventAction *eventAction) : fEventAction(eventAction) {}

  void PreUserTrackingAction(const G4Track *track) override;

  const G4Track *GetTrack() const { return fTrack; }

private:
  EventAction *fEventAction;

  const G4Track *fTrack;
};

#endif // ACTIONS_HH
