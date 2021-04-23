// SPDX-License-Identifier: Apache-2.0

#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

class ActionInitialization final : public G4VUserActionInitialization {
public:
  void BuildForMaster() const override;
  void Build() const override;
};

#endif // ACTION_INITIALIZATION_HH
