#include "RGBLinearInterpolator.hpp"
#include "colorconvert.h"
using namespace ColorConvert;

// template void ColorConvert::DIN2rgb<float, int>(float, float, float, int &,
// int &, int &);

RGBLinearInterpolator::RGBLinearInterpolator(double start_r, double start_g,
                                             double start_b, double end_r,
                                             double end_g, double end_b) {
  rgb2DIN(start_r, start_g, start_b, start_L, start_a, start_b);
  rgb2DIN(end_r, end_g, end_b, end_L, end_a, end_b);
}

std::tuple<float, float, float>
RGBLinearInterpolator::interpolate(float t) const {
  if (t < 0.0f || t > 1.0f) {
    throw std::out_of_range("Interpolation parameter must be between 0 and 1.");
  }
  // Linear interpolation of DIN99 values
  float L = (1 - t) * start_L + t * end_L;
  float a = (1 - t) * start_a + t * end_a;
  float b = (1 - t) * start_b + t * end_b;

  // Convert back to RGB
  float r, g, bl; // 'bl' is used instead of 'b' to avoid name collision with
                  // the member 'b'
  DIN2rgb<float, float>(L, a, b, r, g, bl);
  return std::make_tuple(r, g, bl);
}
