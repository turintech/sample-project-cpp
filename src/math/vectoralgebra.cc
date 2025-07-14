#include "vectoralgebra.h"

#include <cmath>
#include <algorithm>  // For std::max

/** @brief solve a cubic equation
 *
 * @param a Coefficient of x^3
 * @param b Coefficient of x^2
 * @param c Coefficient of x
 * @param d Constant term
 * @param x Vector of x values (input)
 *
 * @return Vector of y values (output)
 */
std::vector<double>
MathVectorAlgebra::Cubic(int a, int b, int c, int d, const std::vector<double> &x) {
  // Minor improvement: Use reserve instead of assignment, avoid constructor overhead
  int n = x.size();
  std::vector<double> y;
  y.reserve(n);
  for (int i = 0; i < n; ++i) {
    double xi = x[i];
    double xi2 = xi * xi;          // compute x[i]^2 once
    y.push_back(a * xi * xi2 + b * xi2 + c * xi + d);  // x^3, x^2, x, d
  }
  return y;
}

/** @brief convolve vector x with smaller vector h
 *
 * offset defined such that h[offset] is the center of h
 *
 * @param offset Offset of h
 * @param h Pointer to array of h values (input)
 * @param x Pointer to array of x values (input)
 *
 * @return Pointer to array of y values (output)
 */
std::vector<double>
MathVectorAlgebra::Convolve(int offset, const std::vector<double> &h, const std::vector<double> &x) {
  int m = h.size();
  int n = x.size();

  // Use double for sum_h to avoid integer overflow/loss of precision for doubles
  double sum_h = 0.0;
  for (int i = 0; i < m; ++i) {
    sum_h += std::abs(h[i]);
  }
  // Avoid division inside innermost loop, precompute normalization factor
  if (sum_h == 0.0) {
    sum_h = 1.0;
  }
  double norm = 1.0 / sum_h;

  // Optimize convolution bounds to avoid redundant bounds checks in inner loop
  std::vector<double> y(n, 0.0);
  for (int i = 0; i < n; ++i) {
    double acc = 0.0;
    // j must be such that i - offset + j is in [0, n-1]
    int j_start = std::max(0, offset - i);
    int j_end = std::min(m, n + offset - i);
    for (int j = j_start; j < j_end; ++j) {
      acc += h[j] * x[i - offset + j];
    }
    y[i] = acc * norm;
  }
  return y;
}