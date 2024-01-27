#ifndef pitch_H
#define pitch_H
#pragma once
#include "plugin.hpp"
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

template <uint MOD> class ZZ_Mod {
private:
  int value;

  uint mod(uint input) const {
    int result = static_cast<int>(input) % static_cast<int>(MOD);
    return result >= 0 ? result : result + MOD;
  }

public:
  ZZ_Mod() : value(0) {}
  explicit ZZ_Mod(uint v) : value(mod(v)) {}

  uint unMod() const { return mod(value); }

  ZZ_Mod operator+(const ZZ_Mod &other) const {
    return ZZ_Mod(value + other.value);
  }
  ZZ_Mod operator-(const ZZ_Mod &other) const {
    return ZZ_Mod(value - other.value);
  }
  ZZ_Mod operator*(const ZZ_Mod &other) const {
    return ZZ_Mod(value * other.value);
  }
  bool operator==(const ZZ_Mod &other) const { return value == other.value; }
  bool operator<(const ZZ_Mod &other) const { return value < other.value; }
  std::string to_string() const {
    return "{" + std::to_string(value) + " % " + std::to_string(MOD) + "}";
  }
};

using ZZ_7 = ZZ_Mod<7>;
using ZZ_12 = ZZ_Mod<12>;

// Define the HeptatonicScale struct using ZZ_12
struct HeptatonicScale {
  std::vector<ZZ_12> notes;

  HeptatonicScale(std::initializer_list<ZZ_12> init) : notes(init) {}

  HeptatonicScale succMode() const {
    HeptatonicScale result = *this;
    ZZ_12 first = result.notes[0];
    for (size_t i = 0; i < result.notes.size() - 1; ++i) {
      result.notes[i] = result.notes[i + 1];
    }
    result.notes[result.notes.size() - 1] = first;
    return result;
  }

  ZZ_12 operator[](int index) const {
    if (index < 0 || index >= static_cast<int>(notes.size()))
      throw std::out_of_range("Index out of range");
    return notes[index];
  }
};
std::string toStringHeptatonicScale(const HeptatonicScale &scale);
HeptatonicScale getScaleByMode(ZZ_7 mode);
// Enum representing the 12-tone chromatic scale
enum class Chromatic { C, Cs, D, Eb, E, F, Fs, G, Gs, A, Bb, B };
// Function to convert Chromatic to string
inline std::string toString(Chromatic note) {
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
inline Chromatic toChromatic(ZZ_12 num) {
  uint n = num.unMod();
  if (n >= 12) {
    throw std::out_of_range(
        "Number must be in the range 0-11 for Chromatic conversion");
  }
  return static_cast<Chromatic>(n);
}

// Function to convert Chromatic to ZZ_12
inline ZZ_12 toZZ_12(Chromatic note) { return ZZ_12(static_cast<uint>(note)); }

#endif // HEADER_FILE_NAME_H