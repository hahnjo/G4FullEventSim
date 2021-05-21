// SPDX-License-Identifier: Apache-2.0

#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include <G4Timer.hh>
#include <G4UserRunAction.hh>

class RunAction : public G4UserRunAction {
public:
  G4Run *GenerateRun() override;
};

class MasterRunAction final : public RunAction {
public:
  void BeginOfRunAction(const G4Run *) override;
  void EndOfRunAction(const G4Run *aRun) override;

private:
  G4Timer fTimer;
};

#endif // RUN_ACTION_HH
