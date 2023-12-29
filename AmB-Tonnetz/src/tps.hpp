
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

  T sum() const {
    T result = T();
    for (const auto &value : values) {
      result += value;
    }
    return result;
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

template <size_t N> std::vector<float> cs(const FixedVector<uint, N> &mods) {
  std::vector<float> result(12, 0.0f); // Assuming the modulo is 12
  for (const auto &mod : mods.values) {
    if (mod < 12) {
      result[mod] = 1.0f;
    }
  }
  return result;
}