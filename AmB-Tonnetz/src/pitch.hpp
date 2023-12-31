#pragma once
#include "plugin.hpp"
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

template <uint MOD> class ZZ_Mod {
private:
  uint value;

  uint mod(int input) const {
    int result = input % MOD;
    return result >= 0 ? result : result + MOD;
  }

public:
  ZZ_Mod() : value(0) {}
  explicit ZZ_Mod(uint v) : value(mod(v)) {}

  uint unMod() const { return value; }

  ZZ_Mod operator+(const ZZ_Mod &other) const {
    return ZZ_Mod(mod(value + other.value));
  }
  ZZ_Mod operator-(const ZZ_Mod &other) const {
    return ZZ_Mod(mod(value - other.value));
  }
  ZZ_Mod operator*(const ZZ_Mod &other) const {
    return ZZ_Mod(mod(value * other.value));
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
std::string toString(Chromatic note);
Chromatic toChromatic(ZZ_12 num);
// Function to convert Chromatic to ZZ_12
ZZ_12 toZZ_12(Chromatic note) { return ZZ_12(static_cast<uint>(note)); }
