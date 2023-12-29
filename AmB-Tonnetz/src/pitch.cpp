
#include "pitch.hpp"
#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "plugin.hpp"
#include "rack.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

std::string toStringHeptatonicScale(const HeptatonicScale &scale) {
  std::string result = "{";
  for (size_t i = 0; i < scale.notes.size(); ++i) {
    result += scale.notes[i].to_string();
    if (i < scale.notes.size() - 1) {
      result += ", ";
    }
  }
  result += "}";
  return result;
}

// Define the scales as global constants
// Define constants for each mode
const HeptatonicScale cIonian = {ZZ_12(0), ZZ_12(2), ZZ_12(4), ZZ_12(5),
                                 ZZ_12(7), ZZ_12(9), ZZ_12(11)};
const HeptatonicScale cDorian = cIonian.succMode();
const HeptatonicScale cPhrygian = cDorian.succMode();
const HeptatonicScale cLydian = cPhrygian.succMode();
const HeptatonicScale cMixolydian = cLydian.succMode();
const HeptatonicScale cAeolian = cMixolydian.succMode();
const HeptatonicScale cLocrian = cAeolian.succMode();

// Function to get scale by mode using switch case
HeptatonicScale getScaleByMode(ZZ_7 mode) {
  switch (mode.unMod()) {
  case 0:
    return cIonian;
  case 1:
    return cDorian;
  case 2:
    return cPhrygian;
  case 3:
    return cLydian;
  case 4:
    return cMixolydian;
  case 5:
    return cAeolian;
  case 6:
    return cLocrian;
  default:
    throw std::out_of_range("Invalid mode");
  }
}

// Function to convert Chromatic to string
std::string toString(Chromatic note) {
  switch (note) {
  case Chromatic::C:
    return "C";
  case Chromatic::Cs:
    return "C#";
  case Chromatic::D:
    return "D";
  case Chromatic::Eb:
    return "Eb";
  case Chromatic::E:
    return "E";
  case Chromatic::F:
    return "F";
  case Chromatic::Fs:
    return "F#";
  case Chromatic::G:
    return "G";
  case Chromatic::Gs:
    return "G#";
  case Chromatic::A:
    return "A";
  case Chromatic::Bb:
    return "Bb";
  case Chromatic::B:
    return "B";
  default:
    throw std::out_of_range("Invalid Chromatic note");
  }
}

// Function to convert uint to Chromatic (assuming 0 corresponds to C)
Chromatic toChromatic(ZZ_12 num) {
  uint n = num.unMod();
  if (n >= 12) {
    throw std::out_of_range(
        "Number must be in the range 0-11 for Chromatic conversion");
  }
  return static_cast<Chromatic>(n);
}

// Function to convert Chromatic to ZZ_12
ZZ_12 toZZ_12(Chromatic note) { return ZZ_12(static_cast<uint>(note)); }