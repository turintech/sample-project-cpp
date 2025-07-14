#ifndef __MATH_SIMULATION_H__
#define __MATH_SIMULATION_H__

#include <vector>

// Convenience includes for artemis
#if __has_include(<CL/sycl.hpp>)
#include <CL/sycl.hpp>
#endif   // __has_include

namespace MathSimulation {

// To reduce memory overhead, always pass by const reference for large containers.
// For runtime and memory efficiency, avoid vector copies unless necessary, and unify parameter order/comment for clarity.

// PseudoGravity: Accepts positions (by const ref), avoids copies, returns updated positions.
std::vector<double> PseudoGravity(const std::vector<double> &positions, double dt, int iterations);

// SimpleGravity: Accepts positions, velocities, and masses (all by const ref), avoids unneeded copies.
// Keeping main functionality unchanged; passing vectors by const ref reduces heap allocations and speeds up parameter passing.
std::vector<double> SimpleGravity(const std::vector<double> &positions, const std::vector<double> &velocities,
                                  const std::vector<double> &masses, double dt, int iterations);

// MandelbrotSet: unchanged. Possibly large output, so return by value. Params are primitive types.
std::vector<int> MandelbrotSet(double cx, double cy, double zoom, int width, int height, int iterations);

// MandelbrotPoint: unchanged, returns a single value.
int MandelbrotPoint(double cx, double cy, int iterations);

};   // namespace MathSimulation

#endif   // !__MATH_SIMULATION_H__