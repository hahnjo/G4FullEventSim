// SPDX-License-Identifier: Apache-2.0

#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <FTFP_BERT.hh>

class PhysicsList final : public FTFP_BERT {
  using Base = FTFP_BERT;

public:
  PhysicsList() : Base(/*verbose=*/0) {}

  void ConstructProcess() override;
};

#endif // PHYSICS_LIST_HH
