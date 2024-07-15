
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

std::string to_mode_string(const ZZ_7 &mode) {
  static const std::vector<std::string> romanNumerals = {"I", "II", "III", "IV",
                                                         "V", "VI", "VII"};
  return romanNumerals.at(mode.unMod());
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
