// SPDX-License-Identifier: Apache-2.0

#include <HepMC3/WriterAscii.h>
#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC3.h>

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./shower_lhe <input.lhe> <out.dat>" << std::endl;
    return 1;
  }

  Pythia8::Pythia pythia;
  pythia.readString("Beams:frameType = 4");
  pythia.readString(std::string("Beams:LHEF = ") + argv[1]);
  pythia.init();

  HepMC3::Pythia8ToHepMC3 toHepMC;
  HepMC3::WriterAscii asciiWriter(argv[2]);

  int num_events = 1;
  if (argc >= 4) {
    num_events = atoi(argv[3]);
  }

  while (pythia.next()) {
    HepMC3::GenEvent event;
    toHepMC.fill_next_event(pythia, &event);
    asciiWriter.write_event(event);
  }

  return 0;
}
