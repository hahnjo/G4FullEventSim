# SPDX-License-Identifier: Apache-2.0

PYTHIA8_LIBS ?= -lpythia8
HEPMC3_LIBS ?= -lHepMC3

generate: generate.cc
	$(CXX) -O2 $< -o $@ $(PYTHIA8_LIBS) $(HEPMC3_LIBS)
