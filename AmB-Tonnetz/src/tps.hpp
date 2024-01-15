
#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "pitch.hpp"
#include "plugin.hpp"
#include "rack.hpp"
#include <Eigen/FFT>
#include <Eigen>
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
FixedVector<std::complex<float>, 12>
naiveDft(const FixedVector<std::complex<float>, 12> &input) {
  Eigen::VectorXcf eigenInput(12);
  for (int i = 0; i < 12; ++i) {
    eigenInput(i) = input.values[i];
  }

  Eigen::FFT<float> fft;
  Eigen::VectorXcf eigenOutput(12);
  fft.fwd(eigenOutput, eigenInput);

  // Eigen::VectorXcf eigenOutput = eigenInput.fft();

  FixedVector<std::complex<float>, 12> output;
  for (int i = 0; i < 12; ++i) {
    output.values[i] = eigenOutput(i);
  }

  return output;
}

// FixedVector<std::complex<float>, 12> naiveDft(const
// FixedVector<std::complex<float>, 12> &input)
// {
//   // PFFFT only supports sizes that are powers of 2, so we pad our array to
//   16 std::array<std::complex<float>, 16> paddedInput;
//   std::copy(input.values.begin(), input.values.end(), paddedInput.begin());
//   std::fill(paddedInput.begin() + 12, paddedInput.end(),
//   std::complex<float>(0.0f, 0.0f));

//   // Initialize PFFFT setup for a 1D complex transform of size 16
//   PFFFT_Setup *setup = pffft_new_setup(16, PFFFT_COMPLEX);
//   if (!setup)
//   {
//     throw std::runtime_error("Failed to initialize PFFFT setup");
//   }

//   // Allocate aligned memory for the transform
//   float *paddedInputData = (float *)pffft_aligned_malloc(16 * 2 *
//   sizeof(float)); float *transformedData = (float *)pffft_aligned_malloc(16 *
//   2 * sizeof(float));

//   // Copy our complex input data into the real and imaginary parts of the
//   aligned input array for (size_t i = 0; i < paddedInput.size(); ++i)
//   {
//     paddedInputData[2 * i] = paddedInput[i].real();
//     paddedInputData[2 * i + 1] = paddedInput[i].imag();
//   }

//   // Perform the forward Fourier transform
//   pffft_transform_ordered(setup, paddedInputData, transformedData, nullptr,
//   PFFFT_FORWARD);

//   // Copy the transformed data back into a FixedVector
//   FixedVector<std::complex<float>, 12> output;
//   for (size_t i = 0; i < 12; ++i)
//   {
//     output.values[i] = std::complex<float>(transformedData[2 * i],
//     transformedData[2 * i + 1]);
//   }

//   // Free the allocated memory and destroy the PFFFT setup
//   pffft_aligned_free(paddedInputData);
//   pffft_aligned_free(transformedData);
//   pffft_destroy_setup(setup);

//   return output;
// }

// FixedVector<std::complex<float>, 12>
// naiveDft(const FixedVector<std::complex<float>, 12> &input)
// {
//   FixedVector<std::complex<float>, 12> output;
//   const size_t N = 12;
//   for (size_t k = 0; k < N; ++k)
//   {
//     std::complex<float> sum(0.0f, 0.0f);
//     for (size_t n = 0; n < N; ++n)
//     {
//       float angle = 2 * M_PI * k * n / N;
//       std::complex<float> w(std::cos(angle), -std::sin(angle));
//       sum += input.values[n] * w;
//     }
//     output.values[k] = sum;
//   }
//   return output;
// }

// Define the list 'ws' as a constant array
const std::array<int, 6> ws = {2, 11, 17, 16, 19, 7};
// const std::array<float, 6> ws = {3, 8, 11.5, 15, 14.5, 7.5};

// Function 'w' to access elements from the 'ws' array
int w(int k) {
  if (k >= 0 && k < ws.size()) {
    return ws[k];
  }
  throw std::out_of_range("Index out of range"); // or handle as you see fit
}

FixedVector<std::complex<float>, 12>
cbar(const FixedVectorTwelveComplex &cs_prime) {
  std::complex<float> sum =
      std::accumulate(cs_prime.values.begin(), cs_prime.values.end(),
                      std::complex<float>(0, 0));
  FixedVector<std::complex<float>, 12> normalized_cs_prime;
  if (sum != std::complex<float>(0, 0)) // Avoid division by zero
  {
    for (size_t i = 0; i < cs_prime.values.size(); ++i) {
      normalized_cs_prime.values[i] = cs_prime.values[i] / sum;
    }
  }
  return normalized_cs_prime;
}

FixedVectorSixComplex tk6_prime(const FixedVectorTwelveComplex &cs_prime) {

  // Perform DFT on the input
  auto dft_result = naiveDft(cbar(cs_prime));
  if (dft_result.values[0] !=
      std::complex<float>(0, 0)) // Check if the first element is not zero
  {
    for (auto &val : dft_result.values) {
      val /= dft_result.values[0]; // Divide each element by the first element
    }
  }

  // Apply the weights and keep only the first 6 elements
  FixedVectorSixComplex result;
  for (size_t i = 1; i < 7; ++i) {
    result.values[i - 1] =
        (std::complex<float>(w(i - 1), 0)) * dft_result.values[i];
  }

  return result;
}

// Function to take the norm of a complex vector
float complexVectorNorm(const std::vector<std::complex<float>> &vec) {
  float norm = 0.0f;
  for (const auto &val : vec) {
    norm += std::norm(val);
  }
  return std::sqrt(norm);
}

FixedVectorSixComplex tk6(const std::vector<ZZ_12> &ns) {
  auto cs_prime =
      cs(ns); // Assuming 'cs' returns a FixedVector of appropriate type
  return tk6_prime(cs_prime);
}

float consonance(const std::vector<ZZ_12> &xs) {
  // Apply tk6 to the pitch classes.
  std::vector<std::complex<float>> tk6Result = (tk6(xs)).to_vector();

  // Calculate the norm of the tk6Result.

  float normValue = complexVectorNorm(tk6Result);

  // Divide by the constant value.
  // const float constant = 32.86335345030997f;
  return normValue; // / constant;
}

float normalizedConsonance(const std::vector<ZZ_12> &xs) {
  float normValue = consonance(xs);
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

  std::complex<float> normsProduct =
      complexVectorNorm(v1) * complexVectorNorm(v2);
  return dotProduct / normsProduct;
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

    float currentNorm = complexVectorNorm(tk6Result);
    maxConsonance = std::max(maxConsonance, currentNorm);
  }

  return maxConsonance;
}
