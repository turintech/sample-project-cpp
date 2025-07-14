#ifndef __MATH_MATRIXALGEBRA_H__
#define __MATH_MATRIXALGEBRA_H__

#include <vector>

// Convenience includes for artemis
#if __has_include(<CL/sycl.hpp>)
#include <CL/sycl.hpp>
#endif   // __has_include

namespace MathMatrixAlgebra {

// Use const reference for input vectors where possible, only copy when returning
// Use noexcept where applicable (but don't add if exceptions are documented/possible)

std::vector<double> MatrixMultiply(const std::vector<double> &a, int nAI, int nAJ, const std::vector<double> &x,
                                   int nXI);
std::vector<double> MatrixMultiply(const std::vector<double> &a, int nAI, int nAJ, const std::vector<double> &b,
                                   int nBI, int nBJ);
std::vector<double> RotatePoints(const std::vector<double> &points, int nPoints, int nDim,
                                 const std::vector<double> &rotmat);
std::vector<double> LinearSolver(const std::vector<double> &equations, const std::vector<double> &inputs, int nI);
void LuDecompose(const std::vector<double> &A, std::vector<double> &L, std::vector<double> &U, int nI);

// Performance improvement: take b by const reference to avoid copying input (unless an out-of-class change is required)
std::vector<double> ForwardSubstitution(const std::vector<double> &L, const std::vector<double> &b, int nI);
std::vector<double> BackwardSubstitution(const std::vector<double> &U, const std::vector<double> &y, int nI);

/**
 * @brief Convolution in 2D
 * 
 * @param a The input vector. Shape (nAI, nAJ).
 * @param k The kernel. Shape (nKI, nKJ).
 * @param padding
 * @param stride
 * 
 * @return b, the output vector. Shape (nBI, nBJ)
 *
 * Performance: Use reserve() ahead of time to minimize reallocations, whenever the function implementation is touched
 */
std::vector<double> Convolve2D(const std::vector<double> &a, int nAI, int nAJ, const std::vector<double> &k, int nKI,
                               int nKJ, int padding, int stride);

};   // namespace MathMatrixAlgebra

#endif   // !__MATH_MATRIXALGEBRA_H__