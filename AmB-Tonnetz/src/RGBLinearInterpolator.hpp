#ifndef RGBLINEARINTERPOLATOR_H
#define RGBLINEARINTERPOLATOR_H

#include <tuple>

struct RGBLinearInterpolator {
  double start_L, start_a, start_b; // Start color in DIN99 representation
  double end_L, end_a, end_b;       // End color in DIN99 representation

  RGBLinearInterpolator(double start_r, double start_g, double start_b,
                        double end_r, double end_g, double end_b);

  std::tuple<float, float, float> interpolate(float t) const;
};

struct DualRGBLinearInterpolator {
  RGBLinearInterpolator positiveInterpolator;
  RGBLinearInterpolator negativeInterpolator;

  DualRGBLinearInterpolator(const RGBLinearInterpolator &posInterp,
                            const RGBLinearInterpolator &negInterp);
  DualRGBLinearInterpolator(double pureOrangeR, double pureOrangeG,
                            double pureOrangeB, double nearlyWhiteGreenR,
                            double nearlyWhiteGreenG, double nearlyWhiteGreenB,
                            double pureVioletR, double pureVioletG,
                            double pureVioletB);

  std::tuple<float, float, float> interpolate(float t) const;
};

#endif // RGBLINEARINTERPOLATOR_H
