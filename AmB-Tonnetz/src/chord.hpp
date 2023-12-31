
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

// Assuming Chromatic and ZZ_12 are already defined

// Helper function to convert Chromatic enum to corresponding pitch class number
int chromaticToPitchClass(Chromatic note) { return static_cast<int>(note); }

std::vector<ZZ_12> constructIntervalRep(const std::vector<Chromatic> &chord) {
  std::vector<ZZ_12> intervals;
  for (size_t i = 1; i < chord.size(); ++i) {
    int interval =
        chromaticToPitchClass(chord[i]) - chromaticToPitchClass(chord[i - 1]);
    intervals.push_back(ZZ_12(interval));
  }
  return intervals;
}

const std::map<std::vector<int>, std::vector<std::string>> tnIndexToChordInfo =
    {
        {{0, 1, 2}, {"ChordName1"}}, {{0, 1, 4}, {"ChordName2"}},
        // ... continue for other chords
};

std::vector<std::string> getChordName(const std::vector<Chromatic> &chord) {
  auto intervals = constructIntervalRep(chord);

  // Convert ZZ_12 intervals to int and normalize to start from 0
  std::vector<int> intervalVector;
  int root = intervals.front().unMod();
  for (const auto &interval : intervals) {
    intervalVector.push_back((interval.unMod() - root + 12) % 12);
  }
  std::sort(intervalVector.begin(),
            intervalVector.end()); // Ensure ordered for map lookup

  // Look up in tnIndexToChordInfo
  auto it = tnIndexToChordInfo.find(intervalVector);
  if (it != tnIndexToChordInfo.end()) {
    return it->second; // Chord name found
  }
  return {"Unknown Chord"}; // No matching chord found
}
