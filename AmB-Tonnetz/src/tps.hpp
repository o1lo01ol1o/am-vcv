
#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "pitch.hpp"
#include "plugin.hpp"
#include "rack.hpp"
#include <complex>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T, size_t N> struct FixedVector {
  std::array<T, N> values;
  FixedVector(std::initializer_list<T> init) {
    if (init.size() != N)
      throw std::length_error("Initializer list must have the same length as "
                              "the FixedVector size.");
    std::copy(init.begin(), init.end(), values.begin());
  }
  FixedVector(T defaultValue = T()) { values.fill(defaultValue); }

  FixedVector operator+(const FixedVector &other) const {
    FixedVector result;
    for (size_t i = 0; i < N; ++i) {
      result.values[i] = values[i] + other.values[i];
    }
    return result;
  }

  FixedVector operator-(const FixedVector &other) const {
    FixedVector result;
    for (size_t i = 0; i < N; ++i) {
      result.values[i] = values[i] - other.values[i];
    }
    return result;
  }

  FixedVector operator*(const FixedVector &other) const {
    FixedVector result;
    for (size_t i = 0; i < N; ++i) {
      result.values[i] = values[i] * other.values[i];
    }
    return result;
  }

  FixedVector operator-() const {
    FixedVector result;
    for (size_t i = 0; i < N; ++i) {
      result.values[i] = -values[i];
    }
    return result;
  }
  bool operator==(const FixedVector &other) const {
    for (size_t i = 0; i < N; ++i) {
      if (values[i] != other.values[i])
        return false;
    }
    return true;
  }
  T &operator[](size_t index) { return values[index]; }

  const T &operator[](size_t index) const { return values[index]; }

  bool operator<(const FixedVector &other) const {
    for (size_t i = 0; i < N; ++i) {
      if (values[i] < other.values[i])
        return true;
      else if (values[i] > other.values[i])
        return false;
    }
    return false;
  }
  std::string to_string() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < N; ++i) {
      oss << values[i];
      if (i < N - 1)
        oss << ", ";
    }
    oss << "]";
    return oss.str();
  }

  T sum() const {
    T result = T();
    for (const auto &value : values) {
      result += value;
    }
    return result;
  }
  std::vector<T> to_vector() const {
    return std::vector<T>(values.begin(), values.end());
  }

  template <typename Func>
  auto map(Func func) const
      -> FixedVector<decltype(func(std::declval<T>())), N> {
    FixedVector<decltype(func(std::declval<T>())), N> result;
    for (size_t i = 0; i < N; ++i) {
      result.values[i] = func(values[i]);
    }
    return result;
  }
};

using FixedVectorSixChromatic = FixedVector<Chromatic, 6>;
using FixedVectorTwelveChromatic = FixedVector<Chromatic, 12>;
using FixedVectorSixFloat = FixedVector<float, 6>;
using FixedVectorTwelveFloat = FixedVector<float, 12>;
using FixedVectorSixUint = FixedVector<uint, 6>;
using FixedVectorTwelveUint = FixedVector<uint, 12>;
using FixedVectorSixComplex = FixedVector<std::complex<float>, 6>;
using FixedVectorTwelveComplex = FixedVector<std::complex<float>, 12>;
const FixedVectorTwelveChromatic chromaticVector();

const FixedVectorTwelveChromatic chromaticVector() {
  return FixedVectorTwelveChromatic{
      {Chromatic::C, Chromatic::Cs, Chromatic::D, Chromatic::Eb, Chromatic::E,
       Chromatic::F, Chromatic::Fs, Chromatic::G, Chromatic::Gs, Chromatic::A,
       Chromatic::Bb, Chromatic::B}};
}

std::complex<float> toComplex(int value) {
  // Implement the conversion from an integer to a complex number.
  // This is a placeholder implementation.
  return std::complex<float>(static_cast<float>(value), 0.0f);
}

FixedVectorTwelveComplex cs(const std::vector<ZZ_12> &mods) {
  FixedVectorTwelveComplex result(std::complex<float>(0.0f, 0.0f));

  for (const auto &mod : mods) {
    uint index = mod.unMod(); // Get the modulus value
    if (index < 12) {
      result.values[index] =
          std::complex<float>(1.0f, 0.0f); // Set to 1 at the index
    }
  }

  return result;
}

FixedVectorSixComplex toTk(const FixedVectorTwelveUint &c) {
  FixedVectorSixComplex result{};
  for (size_t i = 0; i < 6; ++i) {
    // Assuming the conversion to 'Mod' and then to complex number
    // For simplicity, directly converting the integer to a complex number
    result.values[i] = toComplex(c.values[i]);
  }
  return result;
}

FixedVector<std::complex<float>, 12>
dft(const FixedVector<std::complex<float>, 12> &input) {
  // PFFFT only supports sizes that are powers of 2.
  // If 12 is a fixed requirement, you'll need to pad the input to the nearest
  // power of 2.
  const size_t N = 16; // Next power of 2 after 12

  // Allocate aligned memory
  float *in_aligned = (float *)pffft_aligned_malloc(sizeof(float) * 2 * N);
  float *out_aligned = (float *)pffft_aligned_malloc(sizeof(float) * 2 * N);

  // Copy input data to aligned buffer, interleaving real and imaginary parts
  for (size_t i = 0; i < 12; ++i) {
    in_aligned[2 * i] = input.values[i].real();
    in_aligned[2 * i + 1] = input.values[i].imag();
  }
  // Pad the rest with zeros
  std::fill(in_aligned + 24, in_aligned + 2 * N, 0.0f);

  // Create plan
  PFFFT_Setup *setup = pffft_new_setup(N, PFFFT_COMPLEX);
  pffft_transform_ordered(setup, in_aligned, out_aligned, NULL, PFFFT_FORWARD);

  // Copy back to output
  FixedVector<std::complex<float>, 12> output;
  for (size_t i = 0; i < 12; ++i) {
    output.values[i] =
        std::complex<float>(out_aligned[2 * i], out_aligned[2 * i + 1]);
  }

  // Cleanup
  pffft_aligned_free(in_aligned);
  pffft_aligned_free(out_aligned);
  pffft_destroy_setup(setup);

  return output;
}

FixedVector<std::complex<float>, 12>
idft(const FixedVector<std::complex<float>, 12> &input) {
  const size_t N = 16; // Pad to the next power of 2

  // Allocate aligned memory for the input and output
  float *in_aligned = (float *)pffft_aligned_malloc(sizeof(float) * 2 * N);
  float *out_aligned = (float *)pffft_aligned_malloc(sizeof(float) * 2 * N);

  // Copy input data to the aligned buffer, interleaving real and imaginary
  // parts
  for (size_t i = 0; i < 12; ++i) {
    in_aligned[2 * i] = input.values[i].real();
    in_aligned[2 * i + 1] = input.values[i].imag();
  }
  // Pad the rest of the array with zeros
  std::fill(in_aligned + 24, in_aligned + 2 * N, 0.0f);

  // Create the PFFFT setup
  PFFFT_Setup *setup = pffft_new_setup(N, PFFFT_COMPLEX);

  // Perform the IDFT
  pffft_transform_ordered(setup, in_aligned, out_aligned, NULL, PFFFT_BACKWARD);

  // Copy the output from the aligned buffer back to the FixedVector,
  // normalizing it
  FixedVector<std::complex<float>, 12> output;
  for (size_t i = 0; i < 12; ++i) {
    output.values[i] =
        std::complex<float>(out_aligned[2 * i] / N, out_aligned[2 * i + 1] / N);
  }

  // Free the allocated memory and destroy the PFFFT setup
  pffft_aligned_free(in_aligned);
  pffft_aligned_free(out_aligned);
  pffft_destroy_setup(setup);

  return output;
}

// Define the list 'ws' as a constant array
const std::array<int, 6> ws = {2, 11, 17, 16, 19, 7};

// Function 'w' to access elements from the 'ws' array
int w(int k) {
  if (k >= 0 && k < ws.size()) {
    return ws[k];
  }
  throw std::out_of_range("Index out of range"); // or handle as you see fit
}

FixedVectorSixComplex tk6_prime(const FixedVectorTwelveComplex &cs_prime) {
  // Perform DFT on the input
  auto dft_result = dft(cs_prime);

  // Calculate the sum of the input elements (cbar)
  std::complex<float> cbar =
      std::accumulate(cs_prime.values.begin(), cs_prime.values.end(),
                      std::complex<float>(0, 0));

  // Calculate the weights (wks)
  FixedVectorSixComplex wks;
  for (size_t i = 0; i < 6; ++i) {
    wks.values[i] = std::complex<float>(w(i), 0) / cbar;
  }

  // Apply the weights and keep only the first 6 elements
  FixedVectorSixComplex result;
  for (size_t i = 0; i < 6; ++i) {
    result.values[i] = wks.values[i] * dft_result.values[i];
  }

  return result;
}

FixedVectorSixComplex tk6(const std::vector<ZZ_12> &ns) {
  auto cs_prime =
      cs(ns); // Assuming 'cs' returns a FixedVector of appropriate type
  return tk6_prime(cs_prime);
}

float norm(const std::vector<float> &v) {
  float sum = 0.0f;
  for (auto &value : v) {
    sum += value * value;
  }
  return std::sqrt(sum);
}

std::complex<float> complexNorm(const std::vector<std::complex<float>> &v) {
  std::complex<float> sum = 0.0f;
  for (const auto &c : v) {
    sum +=
        (c *
         c); // std::norm computes the squared magnitude of the complex number
  }
  return sqrt(sum);
}

float maxClassConsonance() {
  float maxConsonance = 0.0f;
  FixedVectorTwelveChromatic chromatic = chromaticVector();

  for (size_t i = 0; i < chromatic.values.size(); ++i) {
    // Apply toLocalInterpretation and tk6
    // This is a placeholder. Replace with actual implementation.
    auto toRealPart = [](const std::complex<float> &c) { return c.real(); };
    std::vector<ZZ_12> v = {toZZ_12(chromatic.values[i])};
    std::vector<std::complex<float>> tk6Result = tk6(v).to_vector();

    float currentNorm = toRealPart(complexNorm(tk6Result));
    maxConsonance = std::max(maxConsonance, currentNorm);
  }

  return maxConsonance;
}

float consonance(const std::vector<ZZ_12> &xs) {
  // Apply tk6 to the pitch classes.
  // Replace this with your actual implementation of tk6.
  auto toRealPart = [](const std::complex<float> &c) { return c.real(); };
  std::vector<std::complex<float>> tk6Result = (tk6(xs)).to_vector();

  // Calculate the norm of the tk6Result.
  float normValue = toRealPart(complexNorm(tk6Result));

  // Divide by the constant value.
  const float constant = 32.86335345030997f;
  return normValue / constant;
}

std::complex<float>
euclideanDistance(const std::vector<std::complex<float>> &v1,
                  const std::vector<std::complex<float>> &v2) {
  std::complex<float> sum = 0.0f;
  for (size_t i = 0; i < v1.size(); ++i) {
    sum += std::pow(v1[i] - v2[i], 2);
  }
  return std::sqrt(sum);
}

std::complex<float> cosineDistance(const std::vector<std::complex<float>> &v1,
                                   const std::vector<std::complex<float>> &v2) {
  std::complex<float> dotProduct = std::inner_product(
      v1.begin(), v1.end(), v2.begin(), std::complex<float>(0, 0),
      std::plus<std::complex<float>>(),
      [](const std::complex<float> &a, const std::complex<float> &b) {
        return a * std::conj(b);
      });

  std::complex<float> normsProduct = complexNorm(v1) * complexNorm(v2);
  return dotProduct / normsProduct;
}
