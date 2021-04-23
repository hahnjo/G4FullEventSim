// SPDX-License-Identifier: Apache-2.0

#include <HepMC3/WriterAscii.h>
#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC3.h>

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./generate <pythia8.cmnd> <out.dat>" << std::endl;
    return 1;
  }

  Pythia8::Pythia pythia;
  pythia.readFile(argv[1]);
  pythia.init();
  pythia.next();

  HepMC3::Pythia8ToHepMC3 toHepMC;
  HepMC3::GenEvent event;
  toHepMC.fill_next_event(pythia, &event);
  HepMC3::WriterAscii asciiWriter(argv[2]);
  asciiWriter.write_event(event);

  return 0;
}
