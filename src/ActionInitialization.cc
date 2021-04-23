// SPDX-License-Identifier: Apache-2.0

#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);
}
