# SPDX-License-Identifier: Apache-2.0

PYTHIA8_LIBS ?= -lpythia8
HEPMC3_LIBS ?= -lHepMC3

shower_lhe: shower_lhe.cc
	$(CXX) -O2 $< -o $@ $(PYTHIA8_LIBS) $(HEPMC3_LIBS)
