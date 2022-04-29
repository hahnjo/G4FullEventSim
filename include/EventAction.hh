// SPDX-License-Identifier: Apache-2.0

#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "ParticleStatistics.hh"

#include <G4UserEventAction.hh>

class G4Track;

class EventAction final : public G4UserEventAction {
public:
  void BeginOfEventAction(const G4Event *) override;
  void EndOfEventAction(const G4Event *) override;

  void AccountTrack(const G4Track *track);

private:
  ParticleStatistics fParticleStats;
};

#endif // EVENT_ACTION_HH
