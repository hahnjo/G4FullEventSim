// SPDX-License-Identifier: Apache-2.0

#include "PhysicsList.hh"

#include "PhysicsSettings.hh"

#include <G4DecayPhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4HadronElasticPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT.hh>
#include <G4IonPhysics.hh>
#include <G4NeutronTrackingCut.hh>
#include <G4ProcessManager.hh>
#include <G4StoppingPhysics.hh>
#include <G4SystemOfUnits.hh>

class HadronPhysicsFTFP_BERT final : public G4HadronPhysicsFTFP_BERT {
public:
  HadronPhysicsFTFP_BERT(G4int verbose, const PhysicsSettings &physicsSettings)
      : G4HadronPhysicsFTFP_BERT(verbose), fPhysicsSettings(physicsSettings) {}

  void ConstructProcess() override {
    minFTFP_pion = fPhysicsSettings.FTFP_BERTpion.minFTFP;
    maxBERT_pion = fPhysicsSettings.FTFP_BERTpion.maxBERT;
    minFTFP_kaon = fPhysicsSettings.FTFP_BERTkaon.minFTFP;
    maxBERT_kaon = fPhysicsSettings.FTFP_BERTkaon.maxBERT;
    minFTFP_proton = fPhysicsSettings.FTFP_BERTproton.minFTFP;
    maxBERT_proton = fPhysicsSettings.FTFP_BERTproton.maxBERT;
    minFTFP_neutron = fPhysicsSettings.FTFP_BERTneutron.minFTFP;
    maxBERT_neutron = fPhysicsSettings.FTFP_BERTneutron.maxBERT;

    if (G4Threading::IsMasterThread()) {
      DumpBanner();
    }
    CreateModels();
  }

private:
  const PhysicsSettings &fPhysicsSettings;
};

PhysicsList::PhysicsList(const PhysicsSettings &physicsSettings) {
  // Based on FTFP_BERT
  defaultCutValue = 0.7 * mm;
  G4int ver = 0;

  // EM Physics
  RegisterPhysics(new G4EmStandardPhysics(ver));

  // Synchroton Radiation & GN Physics
  RegisterPhysics(new G4EmExtraPhysics(ver));

  // Decays
  RegisterPhysics(new G4DecayPhysics(ver));

  // Hadron Elastic scattering
  RegisterPhysics(new G4HadronElasticPhysics(ver));

  // Hadron Physics
  RegisterPhysics(new HadronPhysicsFTFP_BERT(ver, physicsSettings));

  // Stopping Physics
  RegisterPhysics(new G4StoppingPhysics(ver));

  // Ion Physics
  RegisterPhysics(new G4IonPhysics(ver));

  // Neutron tracking cut
  RegisterPhysics(new G4NeutronTrackingCut(ver));
}
