#ifndef COLORCONVERT_H
#define COLORCONVERT_H

#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
namespace ColorConvert {

/**
 * Convert L*a*b-values to DIN99-values.
 *
 * @param[in] L L-value
 * @param[in] L a-value
 * @param[in] L b-value
 * @param[out] DIN_L calculated L-value in the DIN99 system.
 * @param[out] DIN_a calculated a-value in the DIN99 system.
 * @param[out] DIN_b calculated b-value in the DIN99 system.
 */
template <class T>
void Lab2DIN(const T L, const T a, const T b, T &DIN_L, T &DIN_a, T &DIN_b);

/**
 * Convert DIN99-values to L*a*b-values.
 *
 * @param[in] L L-value in the DIN99 system
 * @param[in] L a-value in the DIN99 system
 * @param[in] L b-value in the DIN99 system
 * @param[out] DIN_L calculated L-value in the L*a*b system.
 * @param[out] DIN_a calculated a-value in the L*a*b system.
 * @param[out] DIN_b calculated b-value in the L*a*b system.
 */
template <class T>
void DIN2Lab(const T DIN_L, const T DIN_a, const T DIN_b, T &L, T &a, T &b);

/**
 * Convert a RGB-representation of some color to a L*a*b-representation of the
 * same color.
 *
 * @param[in] r red value (0-255).
 * @param[in] g green value (0-255).
 * @param[in] b blue value (0-255).
 * @param[out] dest_L Calculated L-value in the L*a*b-system
 * @param[out] dest_a Calculated a-value in the L*a*b-system
 * @param[out] dest_b Calculated b-value in the L*a*b-system
 */
template <class Source, class T>
void rgb2Lab(const Source r, const Source g, const Source b, T &dest_L,
             T &dest_a, T &dest_b);

/**
 * Convert a RGB-representation of some color to a DIN99-representation of the
 * same color.
 *
 * @param[in] r red value (0-255).
 * @param[in] g green value (0-255).
 * @param[in] b blue value (0-255).
 * @param[out] dest_L Calculated L-value in the DIN99-system
 * @param[out] dest_a Calculated a-value in the DIN99-system
 * @param[out] dest_b Calculated b-value in the DIN99-system
 */
template <class Source, class T>
void rgb2DIN(const Source r, const Source g, const Source b, T &dest_L,
             T &dest_a, T &dest_b);

template <class Vec> Vec rgb2DIN(const Vec &src);

template <class Source, class T>
void DIN2rgb(const Source L, const Source a, const Source b, T &dest_r,
             T &dest_g, T &dest_b);

template <class Vec> Vec DIN2rgb(const Vec &src);

/**
 * Convert a string containing a hexadecimal RGB-representation of some color to
 * a L*a*b-representation of the same color.
 *
 * @param[in] rgb String (hopefully) containing a hexadecimal RGB-representation
 * of a color.
 * @param[out] dest_L Calculated L-value in the L*a*b-system
 * @param[out] dest_a Calculated a-value in the L*a*b-system
 * @param[out] dest_b Calculated b-value in the L*a*b-system
 */
template <class T>
void rgb2Lab(const std::string &rgb, T &dest_L, T &dest_a, T &dest_b);

/**
 * Convert a string containing a hexadecimal RGB-representation of some color to
 * a DIN99-representation of the same color.
 *
 * @param[in] rgb String (hopefully) containing a hexadecimal RGB-representation
 * of a color.
 * @param[out] DIN_L calculated L-value in the DIN99 system.
 * @param[out] DIN_a calculated a-value in the DIN99 system.
 * @param[out] DIN_b calculated b-value in the DIN99 system.
 */
template <class T>
void rgb2DIN(const std::string &rgb, T &DIN_L, T &DIN_a, T &DIN_b);

/**
 * Calculate the square of a float.
 *
 * @param [in] a The value to square.
 * @return a*a
 */
template <class T> T sqr(const T a);

template <class Vec> Vec rgb2Lab(const Vec &src);

template <class Vec> Vec Lab2rgb(const Vec &src);

template <class Vec> Vec DIN2Lab(const Vec &src);

template <class Vec> Vec Lab2DIN(const Vec &src);

template <class Vec> double DINDiff(const Vec &a, const Vec &b);

template <class Source, class T>
void Lab2rgb(const Source L, const Source a, const Source b, T &dest_r,
             T &dest_g, T &dest_b);

/**
 * Read RGB-values from strings containing hexadecimal values like "ff0000" and
 * store them in integer values. This doesn't throw any exceptions, if the
 * string doesn't make sense the behaviour is undefined.
 *
 * @param[in] rgb String containing the hexadecimal representation of the color.
 * @param[out] r Here the value for red is stored.
 * @param[out] g Here the value for green is stored.
 * @param[out] b Here the value for blue is stored.
 */
void string2rgb(const std::string &rgb, int &r, int &g, int &b) {
  r = b = g = 0;
  try {
    r = std::stoi(rgb.substr(0, 2), nullptr, 16);
    g = std::stoi(rgb.substr(2, 2), nullptr, 16);
    b = std::stoi(rgb.substr(4, 2), nullptr, 16);
  } catch (std::exception &e) {
  } // Who cares?
}

/**
 * Convert L*a*b-values to DIN99-values.
 *
 * @param[in] L L-value
 * @param[in] L a-value
 * @param[in] L b-value
 * @param[out] DIN_L calculated L-value in the DIN99 system.
 * @param[out] DIN_a calculated a-value in the DIN99 system.
 * @param[out] DIN_b calculated b-value in the DIN99 system.
 */
template <class T>
void Lab2DIN(const T L, const T a, const T b, T &DIN_L, T &DIN_a, T &DIN_b) {

#define COS16 0.96126169593831886192
#define SIN16 0.27563735581699918561

  DIN_L = 105.51 * std::log(1 + 0.0158 * L);
  const T DIN_e = a * COS16 + b * SIN16;
  const T DIN_f = 0.7 * (-a * SIN16 + b * COS16);

  const T DIN_G = std::sqrt(DIN_e * DIN_e + DIN_f * DIN_f);
  const T k = std::log(1 + 0.045 * DIN_G) / 0.045;

  if (DIN_G <= 0) {
    DIN_a = 0;
    DIN_b = 0;
  } else {
    DIN_a = k * DIN_e / DIN_G;
    DIN_b = k * DIN_f / DIN_G;
  }
}

template void Lab2DIN(double, double, double, double &, double &, double &);

/**
 * Convert DIN99-values to L*a*b-values.
 *
 * @param[in] L L-value in the DIN99 system
 * @param[in] L a-value in the DIN99 system
 * @param[in] L b-value in the DIN99 system
 * @param[out] DIN_L calculated L-value in the L*a*b system.
 * @param[out] DIN_a calculated a-value in the L*a*b system.
 * @param[out] DIN_b calculated b-value in the L*a*b system.
 */
template <class T>
void DIN2Lab(const T DIN_L, const T DIN_a, const T DIN_b, T &L, T &a, T &b) {

  double h_99ef = std::numeric_limits<double>::quiet_NaN();
  if (DIN_a > 0 && DIN_b >= 0) {
    h_99ef = std::atan(DIN_b / DIN_a);
  } else if (DIN_a == 0 && DIN_b > 0) {
    h_99ef = M_PI_2;
  } else if (DIN_a < 0) {
    h_99ef = M_PI + std::atan(DIN_b / DIN_a);
  } else if (DIN_a == 0 && DIN_b < 0) {
    h_99ef = 3 * M_PI_2;
  } else if (DIN_a > 0 && DIN_b <= 0) {
    h_99ef = 2 * M_PI + std::atan(DIN_b / DIN_a);
  } else if (DIN_a == 0 && DIN_b == 0) {
    h_99ef = 0;
  }

  const double C_99 = std::sqrt(DIN_a * DIN_a + DIN_b * DIN_b);
#define k_CH 1
#define k_E 1
  const double G = (std::exp(0.045 * C_99 * k_CH * k_E) - 1.0) / 0.045;
  const double e = G * std::cos(h_99ef);
  const double f = G * std::sin(h_99ef);

  a = e * COS16 - (f / .7) * SIN16;
  b = e * SIN16 + (f / .7) * COS16;
  L = (std::exp(DIN_L * k_E / 105.51) - 1) / 0.0158;
}

template void DIN2Lab<double>(double, double, double, double &, double &,
                              double &);

template <class Source, class T>
void rgb2Lab(const Source r, const Source g, const Source b, T &dest_L,
             T &dest_a, T &dest_b) {
  // Create a matrix from the image.
  cv::Mat M(1, 1, CV_32FC3);
  // Write the normalized values into the matrix
  M.at<cv::Vec3f>(0, 0)[0] = (float)r / 255;
  M.at<cv::Vec3f>(0, 0)[1] = (float)g / 255;
  M.at<cv::Vec3f>(0, 0)[2] = (float)b / 255;

  // Color space conversion
  cv::cvtColor(M, M, cv::COLOR_RGB2Lab);

  // Reading the values from the matrix.
  dest_L = static_cast<T>(M.at<cv::Vec3f>(0, 0)[0]);
  dest_a = static_cast<T>(M.at<cv::Vec3f>(0, 0)[1]);
  dest_b = static_cast<T>(M.at<cv::Vec3f>(0, 0)[2]);
}

template void rgb2Lab<double, double>(const double, const double, const double,
                                      double &, double &, double &);

template <class Source, class T>
void Lab2rgb(const Source L, const Source a, const Source b, T &dest_r,
             T &dest_g, T &dest_b) {
  // Create a matrix from the image.
  cv::Mat M(1, 1, CV_32FC3);
  // Write the normalized values into the matrix
  M.at<cv::Vec3f>(0, 0)[0] = (float)L;
  M.at<cv::Vec3f>(0, 0)[1] = (float)a;
  M.at<cv::Vec3f>(0, 0)[2] = (float)b;

  // Color space conversion
  cv::cvtColor(M, M, cv::COLOR_Lab2RGB);

  // Reading the values from the matrix.
  dest_r = static_cast<T>(255.0 * M.at<cv::Vec3f>(0, 0)[0]);
  dest_g = static_cast<T>(255.0 * M.at<cv::Vec3f>(0, 0)[1]);
  dest_b = static_cast<T>(255.0 * M.at<cv::Vec3f>(0, 0)[2]);
}

template void Lab2rgb(const double, const double, const double, double &,
                      double &, double &);

/**
 * Convert a string containing a hexadecimal RGB-representation of some color to
 * a L*a*b-representation of the same color.
 *
 * @param[in] rgb String (hopefully) containing a hexadecimal RGB-representation
 * of a color.
 * @param[out] dest_L Calculated L-value in the L*a*b-system
 * @param[out] dest_a Calculated a-value in the L*a*b-system
 * @param[out] dest_b Calculated b-value in the L*a*b-system
 */
template <class T>
void rgb2Lab(const std::string &rgb, T &dest_L, T &dest_a, T &dest_b) {
  // Get the R, G and B from the string
  int r = 0, g = 0, b = 0;
  string2rgb(rgb, r, g, b);
  rgb2Lab(r, g, b, dest_L, dest_a, dest_b);
}

template void rgb2Lab<float>(const std::string &, float &, float &, float &);

/**
 * Convert a string containing a hexadecimal RGB-representation of some color to
 * a DIN99-representation of the same color.
 *
 * @param[in] rgb String (hopefully) containing a hexadecimal RGB-representation
 * of a color.
 * @param[out] DIN_L calculated L-value in the DIN99 system.
 * @param[out] DIN_a calculated a-value in the DIN99 system.
 * @param[out] DIN_b calculated b-value in the DIN99 system.
 */
template <class T>
void rgb2DIN(const std::string &rgb, T &DIN_L, T &DIN_a, T &DIN_b) {
  float L = 0, a = 0, b = 0;
  rgb2Lab(rgb, L, a, b);
  Lab2DIN(L, a, b, DIN_L, DIN_a, DIN_b);
}

template <class Source, class T>
void rgb2DIN(const Source r, const Source g, const Source b, T &dest_L,
             T &dest_a, T &dest_b) {
  rgb2Lab(r, g, b, dest_L, dest_a, dest_b);
  Lab2DIN(dest_L, dest_a, dest_b, dest_L, dest_a, dest_b);
}

template void rgb2DIN(double, double, double, double &, double &, double &);

template <class Vec> Vec rgb2DIN(const Vec &src) {
  Vec dst;
  rgb2DIN(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
  return dst;
}

template cv::Vec3d rgb2DIN(const cv::Vec3d &);
template cv::Vec3f rgb2DIN(const cv::Vec3f &);
template cv::Scalar rgb2DIN(const cv::Scalar &);

template <class Source, class T>
void DIN2rgb(const Source L, const Source a, const Source b, T &dest_r,
             T &dest_g, T &dest_b) {
  DIN2Lab(L, a, b, dest_r, dest_g, dest_b);
  Lab2rgb(dest_r, dest_g, dest_b, dest_r, dest_g, dest_b);
}

template void DIN2rgb(double, double, double, double &, double &, double &);

template <class Vec> Vec DIN2rgb(const Vec &src) {
  Vec dst;
  DIN2rgb(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
  return dst;
}

template cv::Vec3d DIN2rgb(const cv::Vec3d &);
template cv::Vec3f DIN2rgb(const cv::Vec3f &);
template cv::Scalar DIN2rgb(const cv::Scalar &);

/**
 * Calculate the square of a float.
 *
 * @param [in] a The value to square.
 * @return a*a
 */
template <class T> T sqr(const T a) { return a * a; }

/**
 * Calculate the difference of two colors in the L*a*b-system.
 *
 * @param[in] x String containing a hexadecimal RGB-representation of color #1.
 * @param[in] y String containing a hexadecimal RGB-representation of color #2.
 * @return Color difference
 */
double labDiff(const std::string &x, const std::string &y) {

  double x_L = 0, x_a = 0, x_b = 0, y_L = 0, y_a = 0, y_b = 0;

  rgb2Lab(x, x_L, x_a, x_b);
  rgb2Lab(y, y_L, y_a, y_b);

  return std::sqrt(sqr(x_L - y_L) + sqr(x_a - y_a) + sqr(x_b - y_b));
}

/**
 * Calculate the difference of two colors in the DIN99-system.
 *
 * @param[in] x String containing a hexadecimal RGB-representation of color #1.
 * @param[in] y String containing a hexadecimal RGB-representation of color #2.
 * @return Color difference
 */
double DINDiff(const std::string &x, const std::string &y) {

  float x_L = 0, x_a = 0, x_b = 0, y_L = 0, y_a = 0, y_b = 0;

  rgb2DIN(x, x_L, x_a, x_b);
  rgb2DIN(y, y_L, y_a, y_b);

  return std::sqrt(sqr(x_L - y_L) + sqr(x_a - y_a) + sqr(x_b - y_b));
}

template <class Vec> Vec rgb2Lab(const Vec &src) {
  Vec result;
  rgb2Lab(src[0], src[1], src[2], result[0], result[1], result[2]);
  return result;
}

template cv::Vec3f rgb2Lab(const cv::Vec3f &);
template cv::Vec3d rgb2Lab(const cv::Vec3d &);
template cv::Scalar rgb2Lab(const cv::Scalar &);

template <class Vec> Vec Lab2rgb(const Vec &src) {
  Vec result;
  Lab2rgb(src[0], src[1], src[2], result[0], result[1], result[2]);
  return result;
}

template cv::Vec3f Lab2rgb(const cv::Vec3f &);
template cv::Vec3d Lab2rgb(const cv::Vec3d &);
template cv::Scalar Lab2rgb(const cv::Scalar &);

template <class Vec> Vec DIN2Lab(const Vec &src) {
  Vec result;
  DIN2Lab(src[0], src[1], src[2], result[0], result[1], result[2]);
  return result;
}

template cv::Vec3f DIN2Lab(const cv::Vec3f &);
template cv::Vec3d DIN2Lab(const cv::Vec3d &);
template cv::Scalar DIN2Lab(const cv::Scalar &);

template <class Vec> Vec Lab2DIN(const Vec &src) {
  Vec result;
  Lab2DIN(src[0], src[1], src[2], result[0], result[1], result[2]);
  return result;
}

template cv::Vec3f Lab2DIN(const cv::Vec3f &);
template cv::Vec3d Lab2DIN(const cv::Vec3d &);
template cv::Scalar Lab2DIN(const cv::Scalar &);

template <class Vec> double DINDiff(const Vec &a, const Vec &b) {
  const Vec DIN_DIFF = rgb2DIN(a) - rgb2DIN(b);
  return std::sqrt(DIN_DIFF[0] * DIN_DIFF[0] + DIN_DIFF[1] * DIN_DIFF[1] +
                   DIN_DIFF[2] * DIN_DIFF[2]);
}

template double DINDiff(const cv::Vec3f &, const cv::Vec3f &);
template double DINDiff(const cv::Vec3d &, const cv::Vec3d &);
template double DINDiff(const cv::Scalar &, const cv::Scalar &);
} // namespace ColorConvert

#endif // COLORCONVERT_H