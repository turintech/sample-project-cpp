#include "vectoralgebra.h"

#include <cmath>

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
  int n = x.size();
  std::vector<double> y = std::vector<double>(n);
  for (int i = 0; i < n; i += 1) {
    y[i] = a * x[i] * x[i] * x[i] + b * x[i] * x[i] + c * x[i] + d;
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
  int sum_h = 0;
  std::vector<double> y = std::vector<double>(n);
  for (int i = 0; i < m; i += 1) {
    sum_h += std::abs(h[i]);
  }
  if (sum_h == 0) {
    sum_h = 1;
  }
  for (int i = 0; i < n; i += 1) {
    y[i] = 0;
    for (int j = 0; j < m; j += 1) {
      if (i - offset + j >= 0 && i - offset + j < n) {
        y[i] += h[j] * x[i - offset + j] / sum_h;
      }
    }
  }
  return y;
}
