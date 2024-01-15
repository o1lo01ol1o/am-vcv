
#include "tps.hpp"
#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "../../sdk/Rack-SDK/dep/include/pffft.h"
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

bool prop_euclidean_to_origin_is_l2_norm(ZZ_12 v) {
  std::vector<std::complex<float>> cv =
      cs((FixedVector<ZZ_12, 1>({v})).to_vector()).to_vector();
  std::vector<std::complex<float>> zeroVec(cv.size(),
                                           std::complex<float>(0, 0));

  std::complex<float> euclideanDist = euclideanDistance(cv, zeroVec);
  std::complex<float> l2Norm = complexVectorNorm(cv);

  // Comparing the magnitudes as the result is a complex number
  return euclideanDist == l2Norm;
}

// Function to generate all combinations recursively
void generateCombinations(std::vector<std::vector<ZZ_12>> &result,
                          std::vector<ZZ_12> &current, int maxLength,
                          int start = 0) {
  if (current.size() > 0 && current.size() <= maxLength) {
    result.push_back(current);
  }
  if (current.size() == maxLength) {
    return;
  }

  for (int i = start; i < 12; ++i) {
    current.push_back(ZZ_12(i));
    generateCombinations(result, current, maxLength, i);
    current.pop_back();
  }
}

std::vector<std::vector<ZZ_12>> getAllCombinations(int maxLength) {
  std::vector<std::vector<ZZ_12>> combinations;
  std::vector<ZZ_12> current;
  generateCombinations(combinations, current, maxLength);
  return combinations;
}

// Metric function type: takes two vectors of std::complex<float> and returns

// std::complex<float>
using MetricFunction = std::function<std::complex<float>(
    const std::vector<std::complex<float>> &,
    const std::vector<std::complex<float>> &)>;

void findMinMaxMetric(MetricFunction metric) {

  std::vector<std::vector<ZZ_12>> allVectors = getAllCombinations(12);
  float minDistance = std::numeric_limits<float>::max();
  float maxDistance = 0;

  for (size_t i = 0; i < allVectors.size(); ++i) {
    for (size_t j = i + 1; j < allVectors.size(); ++j) {
      auto dft1 = tk6(allVectors[i]).to_vector();
      auto dft2 = tk6(allVectors[j]).to_vector();
      float distance = std::abs(metric(dft1, dft2));

      if (distance < minDistance) {
        minDistance = distance;
      }
      if (distance > maxDistance) {
        maxDistance = distance;
      }
    }
  }

  std::cout << "Minimum Distance: " << minDistance << std::endl;
  std::cout << "Maximum Distance: " << maxDistance << std::endl;
}

void testDft() {

  const std::array<std::complex<float>, 12> expected = {
      std::complex<float>(1.0f, 0.0f),
      std::complex<float>(0.942728f, -0.232362f),
      std::complex<float>(0.784032f, -0.411492f),
      std::complex<float>(0.560268f, -0.496354f),
      std::complex<float>(0.322698f, -0.467508f),
      std::complex<float>(0.125745f, -0.331561f),
      std::complex<float>(0.0145291f, -0.119658f),
      std::complex<float>(0.0145291f, 0.119658f),
      std::complex<float>(0.125745f, 0.331561f),
      std::complex<float>(0.322698f, 0.467508f),
      std::complex<float>(0.560268f, 0.496354f),
      std::complex<float>(0.784032f, 0.411492f)};
  FixedVector<std::complex<float>, 12> input = {
      std::complex<float>(0.5f, 0.0f), std::complex<float>(0.5f, 0.0f),
      std::complex<float>(0.0f, 0.0f), std::complex<float>(0.0f, 0.0f),
      std::complex<float>(0.0f, 0.0f), std::complex<float>(0.0f, 0.0f),
      std::complex<float>(0.0f, 0.0f), std::complex<float>(0.0f, 0.0f),
      std::complex<float>(0.0f, 0.0f), std::complex<float>(0.0f, 0.0f),
      std::complex<float>(0.0f, 0.0f), std::complex<float>(0.0f, 0.0f)};
  const std::array<std::complex<float>, 12> out = naiveDft(input).values;
  float tolerance = 0.5f; // Define a suitable tolerance value
  for (size_t i = 0; i < out.size(); ++i) {
    if (std::abs(out[i].real() - expected[i].real()) > tolerance ||
        std::abs(out[i].imag() - expected[i].imag()) > tolerance) {
      std::cerr << "DFT output does not match expected value at index " << i
                << std::endl;
      std::cerr << "Expected: " << expected[i] << ", got: " << out[i]
                << std::endl;
      throw std::runtime_error("DFT output mismatch");
    }
  }
  std::cout << "All DFT output values match the expected output within the "
               "given tolerance."
            << std::endl;
}

void testTk6Prime() {
  FixedVector<Chromatic, 2> intervalVector({Chromatic::C, Chromatic::Cs});
  std::vector<ZZ_12> pitches = intervalVector.map(toZZ_12).to_vector();
  FixedVectorTwelveComplex cs_values = cs(pitches);
  FixedVectorSixComplex tk6_prime_result = tk6_prime(cs_values);

  const std::vector<std::complex<float>> expected_result = {
      std::complex<float>(2.00f, 0.00f),   std::complex<float>(10.26f, -2.75f),
      std::complex<float>(12.75f, -7.36f), std::complex<float>(8.00f, -8.00f),
      std::complex<float>(4.75f, -8.23f),  std::complex<float>(0.47f, -1.75f)};

  for (size_t i = 0; i < tk6_prime_result.values.size(); ++i) {
    if (std::abs(tk6_prime_result.values[i].real() -
                 expected_result[i].real()) > 3.9f ||
        std::abs(tk6_prime_result.values[i].imag() -
                 expected_result[i].imag()) > 3.9f) {
      std::cerr << "tk6_prime output does not match expected value at index "
                << i << std::endl;
      std::cerr << "Expected: " << expected_result[i]
                << ", got: " << tk6_prime_result.values[i] << std::endl;
      throw std::runtime_error("tk6_prime output mismatch");
    }
  }
  std::cout << "All tk6_prime output values match the expected output within "
               "the given tolerance."
            << std::endl;
}

void testConsonanceIntervals() {

  float CONST_CONSONANCE_m2 = 18.09;
  float CONST_CONSONANCE_M2 = 20.41;
  float CONST_CONSONANCE_m3 = 24.15;
  float CONST_CONSONANCE_M3 = 22.88;
  float CONST_CONSONANCE_P4 = 25.23;
  float CONST_CONSONANCE_TRITONE = 20.64;

  std::vector<std::pair<Chromatic, float>> intervalsConsonance = {
      {Chromatic::C, 32.86335345030997f},
      {Chromatic::Cs, CONST_CONSONANCE_m2},
      {Chromatic::D, CONST_CONSONANCE_M2},
      {Chromatic::Eb, CONST_CONSONANCE_m3},
      {Chromatic::E, CONST_CONSONANCE_M3},
      {Chromatic::F, CONST_CONSONANCE_P4},
      {Chromatic::Fs, CONST_CONSONANCE_TRITONE}};

  Chromatic baseNote = Chromatic::C;
  float tolerance = 0.1f; // Adjust tolerance as needed.

  for (const auto &interval : intervalsConsonance) {
    FixedVector<Chromatic, 2> intervalVector({baseNote, interval.first});
    std::vector<ZZ_12> pitches = intervalVector.map(toZZ_12).to_vector();
    if (pitches.size() != 2) {
      throw std::runtime_error(
          "Assertion failed: pitches vector must contain exactly two notes.");
    }
    float actualConsonance = consonance(pitches);
    if (std::abs(interval.second - actualConsonance) < tolerance) {
      std::cout << "Consonance test passed for interval: " << toString(baseNote)
                << " to " << toString(interval.first) << std::endl;
    } else {
      std::cout << "Consonance test failed for interval: " << toString(baseNote)
                << " to " << toString(interval.first)
                << ". Expected: " << interval.second
                << ", Actual: " << actualConsonance << std::endl;
    }
  }
}

#ifdef TEST

void testConsonance() {
  // Representing the pitches [C, F, Bb] - adjust these values to your actual
  // pitch representation.
  std::vector<ZZ_12> pitches =
      FixedVector<Chromatic, 3>({Chromatic::C, Chromatic::F, Chromatic::Bb})
          .map(toZZ_12)
          .to_vector();
  // Print the value of pitches
  for (const auto &pitch : pitches) {
    std::cout << pitch.to_string() << " ";
  }
  std::cout << cs(pitches).to_string() << " ";
  float expectedValue = 0.13600764220861314f;
  float tolerance = 0.001f; // Adjust tolerance as needed.

  float actualValue = consonance(pitches);

  std::cout << "Expected value: " << expectedValue
            << ", Actual value: " << actualValue << std::endl;

  if (std::abs(expectedValue - actualValue) < tolerance) {
    std::cout << "Test passed." << std::endl;
  } else {
    std::cout << "Consonance Test failed." << std::endl;
  }
}

void testMaxClassConsonance() {
  float expectedValue = 32.86335345030997f;
  float tolerance = 0.001f; // Adjust tolerance as needed

  float actualValue = maxClassConsonance();

  std::cout << "Expected value: " << expectedValue
            << ", Actual value: " << actualValue << std::endl;

  if (std::abs(expectedValue - actualValue) < tolerance) {
    std::cout << "Test passed." << std::endl;
  } else {
    std::cout << "MaxClassConsonance Test failed." << std::endl;
  }
}
void testCsFunction() {
  // Initialize the vector with chromatic values
  FixedVectorTwelveChromatic chromaticValues = chromaticVector();

  // Convert chromatic values to uint using the map function
  auto toUint = [](Chromatic c) { return static_cast<uint>(c); };
  auto toRealPart = [](const std::complex<float> &c) { return c.real(); };
  auto uintValues = chromaticValues.map(toZZ_12).to_vector();

  // Apply the cs function
  FixedVectorTwelveFloat csResult = (cs(uintValues)).map(toRealPart);

  // Expected result
  FixedVectorTwelveFloat expectedCs(1.0f);
  if (!(csResult == expectedCs)) {
    std::cout << "csResult: " << csResult.to_string() << std::endl;
    std::cout << "expectedCs: " << expectedCs.to_string() << std::endl;
  }
  // Check the result
  assert(csResult == expectedCs);

  std::cout << "Test passed: cs function works as expected." << std::endl;
}

int main() {
  testCsFunction();
  testMaxClassConsonance();
  testDft();
  testConsonanceIntervals();
  assert(prop_euclidean_to_origin_is_l2_norm(toZZ_12(Chromatic::C)));
  // findMinMaxMetric(euclideanDistance);
  // findMinMaxMetric(cosineDistance);
  return 0;
}
#endif