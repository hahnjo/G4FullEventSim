// SPDX-License-Identifier: Apache-2.0

#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "ParticleStatistics.hh"
#include "StepStatistics.hh"

#include <G4UserEventAction.hh>

struct OutputSettings;

class G4Step;
class G4Track;

class EventAction final : public G4UserEventAction {
public:
  EventAction(const OutputSettings &outputSettings);

  void BeginOfEventAction(const G4Event *) override;
  void EndOfEventAction(const G4Event *) override;

  void AccountTrack(const G4Track *track);
  void AccountStep(const G4Step *step);

private:
  ParticleStatistics fParticleStats;
  StepStatistics fStepStats;
  const OutputSettings &fOutputSettings;
};

#endif // EVENT_ACTION_HH
