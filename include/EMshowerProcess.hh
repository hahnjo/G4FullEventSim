// SPDX-License-Identifier: Apache-2.0

#ifndef EMSHOWER_PROCESS_HH
#define EMSHOWER_PROCESS_HH

#include <G4ParticleChange.hh>
#include <G4Region.hh>
#include <G4VDiscreteProcess.hh>

#include <memory>
#include <string>
#include <vector>

class EMshowerProcessMessenger;

class EMshowerProcess final : public G4VDiscreteProcess {
  using Base = G4VDiscreteProcess;
  friend class EMshowerProcessMessenger;

public:
  EMshowerProcess(const G4String &aName = "emShower");

  G4double
  PostStepGetPhysicalInteractionLength(const G4Track &track,
                                       G4double previousStepSize,
                                       G4ForceCondition *condition) override;

protected:
  G4double GetMeanFreePath(const G4Track &track, G4double previousStepSize,
                           G4ForceCondition *condition) override;

public:
  G4VParticleChange *PostStepDoIt(const G4Track &track,
                                  const G4Step &step) override;

private:
  void DumpParticles() const;

public:
  void EndTracking() override;

private:
  G4ParticleChange fParticleChange;
  std::unique_ptr<EMshowerProcessMessenger> fMessenger;

  const G4Region *fRegion = nullptr;
  double fEnergyThreshold = -1;
  bool fStopParticles = false;
  std::string fDumpParticles;

  struct SavedTrack {
    double x, y, z;
    double px, py, pz, e;
    double m;
    int pdg;

    SavedTrack(double x_, double y_, double z_, double px_, double py_,
               double pz_, double e_, double m_, int pdg_)
        : x(x_), y(y_), z(z_), px(px_), py(py_), pz(pz_), e(e_), m(m_),
          pdg(pdg_) {}
  };
  std::vector<SavedTrack> fSavedTracks;
};

#endif // EMSHOWER_PROCESS_HH
