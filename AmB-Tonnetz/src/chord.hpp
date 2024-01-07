
#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "pitch.hpp"
#include "plugin.hpp"
#include "rack.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

class ChordTablesException : public std::exception {
private:
  std::string message;

public:
  ChordTablesException(const std::string &msg) : message(msg) {}

  virtual const char *what() const throw() { return message.c_str(); }
};

// Defining a structure for Chord Representation
struct ChordRep {
  std::vector<int> intervalTuple;  // Pitch class intervals
  std::vector<int> intervalCount;  // Canonical 6-tuple for intervals
  std::vector<int> additionalInfo; // Additional chord information
  int extraData;                   // Additional data (if needed)
  int &operator[](size_t idx);     // Declaration of the operator[]
};

struct TNIStructure {
  std::vector<int> pitchClasses;
  std::tuple<int, int, int, int, int, int> icv;
  std::tuple<int, int, int, int, int, int, int, int> invarianceVector;
  int zRelation;

  TNIStructure(
      std::vector<int> _pitchClasses,
      std::tuple<int, int, int, int, int, int> _icv,
      std::tuple<int, int, int, int, int, int, int, int> _invarianceVector,
      int _zRelation)
      : pitchClasses(_pitchClasses), icv(_icv),
        invarianceVector(_invarianceVector), zRelation(_zRelation) {}
};

extern std::vector<std::string> getChordNames(const Chromatic &root,
                                              const std::set<Chromatic> &chord);
