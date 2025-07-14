#include "matrixalgebra.h"
#include <stdexcept>

/** @brief Multiply a matrix with a vector
 *
 * @param a Matrix shape(nAI, nAJ)
 * @param x Vector shape(nXI)
 *
 * @return Product of a and b, shape(nAI)
 */
std::vector<double>
MathMatrixAlgebra::MatrixMultiply(const std::vector<double> &a, int nAI, int nAJ, const std::vector<double> &x,
                                  int nXI) {
  // Fast dimension checks
  if (nXI != static_cast<int>(x.size())) {
    throw std::invalid_argument("Vector dimensions don't match for `x`");
  }
  if (nAI * nAJ != static_cast<int>(a.size())) {
    throw std::invalid_argument("Matrix dimensions don't match for `a`");
  }
  if (nAJ != nXI) {
    throw std::invalid_argument("Matrix dimensions don't match for multiplication");
  }

  std::vector<double> result(nAI, 0.0);

  // Cache-friendly, more efficient loop order
  for (int i = 0; i < nAI; i++) {
    const int row_start = i * nAJ;
    double sum = 0.0;
    for (int j = 0; j < nAJ; ++j) {
      sum += a[row_start + j] * x[j];
    }
    result[i] = sum;
  }
  return result;
}

/** @brief Multiply two matrices
 *
 * @param a First matrix shape(nAI, nAJ)
 * @param b Second matrix shape(nBI, nBJ)
 *
 * @return Product of a and b, shape(nAI, nBJ)
 */
std::vector<double>
MathMatrixAlgebra::MatrixMultiply(const std::vector<double> &a, int nAI, int nAJ, const std::vector<double> &b, int nBI,
                                  int nBJ) {
  // Fast dimension checks
  if (nAI * nAJ != static_cast<int>(a.size())) {
    throw std::invalid_argument("Matrix dimensions don't match for `a`");
  }
  if (nBI * nBJ != static_cast<int>(b.size())) {
    throw std::invalid_argument("Matrix dimensions don't match for `b`");
  }
  if (nAJ != nBI) {
    throw std::invalid_argument("Matrix dimensions don't match for multiplication");
  }

  std::vector<double> result(nAI * nBJ, 0.0);

  // Optimized for locality - j outer, i middle, k inner loops for cache locality
  for (int j = 0; j < nBJ; ++j) {
    for (int k = 0; k < nAJ; ++k) {
      const double b_val = b[k * nBJ + j];
      for (int i = 0; i < nAI; ++i) {
        result[i * nBJ + j] += a[i * nAJ + k] * b_val;
      }
    }
  }

  return result;
}

/** @brief Rotates a series of points by a matrix
 *
 * @param a Points shape(nPoints, nDim)
 * @param b Rotation matrix shape(nDim, nDim)
 *
 * @return Updated Point Locations shape(nDim, nPoints)
 */
std::vector<double>
MathMatrixAlgebra::RotatePoints(const std::vector<double> &points, int nPoints, int nDim,
                                const std::vector<double> &rotmat) {
  if (nPoints * nDim != static_cast<int>(points.size())) {
    throw std::invalid_argument("Matrix dimensions don't match for `points`");
  }
  if (nDim * nDim != static_cast<int>(rotmat.size())) {
    throw std::invalid_argument("Matrix dimensions don't match for `rotmat`");
  }

  std::vector<double> result(nPoints * nDim, 0.0);

  // Avoid unnecessary vector allocation by using stack array
  for (int i = 0; i < nPoints; ++i) {
    for (int j = 0; j < nDim; ++j) {
      double sum = 0.0;
      for (int k = 0; k < nDim; ++k) {
        sum += rotmat[j * nDim + k] * points[i * nDim + k];
      }
      result[i * nDim + j] = sum;
    }
  }

  return result;
}

/** @brief Solves a set of linear equations
 *
 * @NOTE this doesn't handle many of the edge cases
 * @param equations, the equations as a flattened vector, shape (nI, nI)
 * @param inputs, the input values (solutions to the linear equations), shape (nI)
 * @param nI, the number of equations
 *
 * @return x, the unknowns of the linear equations
 */
std::vector<double>
MathMatrixAlgebra::LinearSolver(const std::vector<double> &equations, const std::vector<double> &inputs, int nI) {
  // Pre-allocate L and U with target size to avoid extra allocations
  std::vector<double> L(nI * nI, 0.0);
  std::vector<double> U(nI * nI, 0.0);

  LuDecompose(equations, L, U, nI);
  std::vector<double> y = ForwardSubstitution(L, inputs, nI);
  std::vector<double> x = BackwardSubstitution(U, y, nI);
  return x;
}

/** @brief LU Decomposition algorithm without pivoting (doolittle)
 *
 * @param A input matrix
 * @param L output lower triangular (to be initialised in this function)
 * @param U output upper triangular (to be initialised in this function)
 */
void
MathMatrixAlgebra::LuDecompose(const std::vector<double> &A, std::vector<double> &L, std::vector<double> &U, int nI) {
  if (nI * nI != static_cast<int>(A.size())) {
    throw std::invalid_argument("invalid dimensions for `A`");
  }

  // Ensure L and U were already allocated as required, clear if not
  if (static_cast<int>(L.size()) != nI * nI) L.assign(nI * nI, 0.0);
  if (static_cast<int>(U.size()) != nI * nI) U.assign(nI * nI, 0.0);

  for (int i = 0; i < nI; i++) {
    // Upper Triangular
    for (int k = i; k < nI; k++) {
      double sum = 0.0;
      for (int j = 0; j < i; j++) {
        sum += L[i * nI + j] * U[j * nI + k];
      }
      U[i * nI + k] = A[i * nI + k] - sum;
    }

    // Lower Triangular
    L[i * nI + i] = 1.0; // Diagonal
    for (int k = i + 1; k < nI; k++) {
      double sum = 0.0;
      for (int j = 0; j < i; j++) {
        sum += L[k * nI + j] * U[j * nI + i];
      }
      L[k * nI + i] = (A[k * nI + i] - sum) / U[i * nI + i];
    }
  }
}

std::vector<double>
MathMatrixAlgebra::ForwardSubstitution(const std::vector<double> &L, const std::vector<double> &b, int nI) {
  if (nI * nI != static_cast<int>(L.size())) {
    throw std::invalid_argument("invalid dimensions for `L`");
  }
  if (nI != static_cast<int>(b.size())) {
    throw std::invalid_argument("invalid dimension for `b`");
  }

  std::vector<double> y(nI);

  for (int i = 0; i < nI; ++i) {
    double sum = b[i];
    for (int j = 0; j < i; ++j) {
      sum -= L[i * nI + j] * y[j];
    }
    if (L[i * nI + i] == 0) {
      throw std::invalid_argument("L needs to be non-singular");
    }
    y[i] = sum / L[i * nI + i];
  }

  return y;
}

std::vector<double>
MathMatrixAlgebra::BackwardSubstitution(const std::vector<double> &U, const std::vector<double> &y, int nI) {
  if (nI * nI != static_cast<int>(U.size())) {
    throw std::invalid_argument("invalid dimensions for `U`");
  }
  if (nI != static_cast<int>(y.size())) {
    throw std::invalid_argument("invalid dimension for `y`");
  }

  std::vector<double> x(nI);

  for (int i = nI - 1; i >= 0; --i) {
    double sum = y[i];
    for (int j = i + 1; j < nI; ++j) {
      sum -= U[i * nI + j] * x[j];
    }
    if (U[i * nI + i] == 0) {
      throw std::invalid_argument("L needs to be non-singular");
    }
    x[i] = sum / U[i * nI + i];
  }

  return x;
}

/** @brief Convolution in 2D
 *
 * @param a, the input vector. Shape (nAI, nAJ).
 * @param k, the kernel. Shape (nKI, nKJ).
 * @param padding
 * @param stride
 *
 * @return b, the output vector. Shape (nBI, nBJ)
 */
std::vector<double>
MathMatrixAlgebra::Convolve2D(const std::vector<double> &a, int nAI, int nAJ, const std::vector<double> &k, int nKI,
                              int nKJ, int padding, int stride) {
  if (nAI * nAJ != static_cast<int>(a.size())) {
    throw std::invalid_argument("invalid dimensions for `a`");
  }
  if (nKI * nKJ != static_cast<int>(k.size())) {
    throw std::invalid_argument("invalid dimensions for `k`");
  }

  const int nBI = (nAI - nKI + 2 * padding) / stride + 1;
  const int nBJ = (nAJ - nKJ + 2 * padding) / stride + 1;
  std::vector<double> b(nBI * nBJ, 0.0);

  for (int by = 0; by < nBI; ++by) {
    for (int bx = 0; bx < nBJ; ++bx) {
      double sum = 0.0;
      int ay = by * stride - padding;
      int ax = bx * stride - padding;
      for (int ky = 0; ky < nKI; ++ky) {
        int iy = ay + ky;
        if (iy < 0 || iy >= nAI) continue;
        for (int kx = 0; kx < nKJ; ++kx) {
          int ix = ax + kx;
          if (ix < 0 || ix >= nAJ) continue;
          sum += a[iy * nAJ + ix] * k[ky * nKJ + kx];
        }
      }
      b[by * nBJ + bx] = sum;
    }
  }

  return b;
}