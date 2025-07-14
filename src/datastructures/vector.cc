#include "vector.h"

#include <iostream>
#include <algorithm> // For std::sort, std::reverse, std::copy, std::rotate

void
OpsVector::PrintVector(const std::vector<int> &v) {
  std::cout << "[";
  for (int i = 0; i < (int) v.size(); i += 1) {
    std::cout << v[i];
    if (i < (int) v.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]";
  std::cout << std::endl;
}

void
OpsVector::PrintVector(const std::vector<double> &v) {
  std::cout << "[";
  for (int i = 0; i < (int) v.size(); i += 1) {
    std::cout << v[i];
    if (i < (int) v.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]";
  std::cout << std::endl;
}

void
OpsVector::PrintMatrix(const std::vector<std::vector<double>> &m) {
  std::cout << "[" << std::endl;
  for (int i = 0; i < (int) m.size(); i += 1) {
    std::cout << "  ";
    PrintVector(m[i]);
  }
  std::cout << "]" << std::endl;
}

/**
 * @brief Add 1 to each element in the vector.
 *
 * @param v Vector to modify.
 * @return The modified vector.
 */
std::vector<int>
OpsVector::ModifyVector(std::vector<int> &v) {
  std::vector<int> v_copy(v);
  for (int i = 0, n = (int)v_copy.size(); i < n; ++i) {
    v_copy[i] += 1;
  }
  return v_copy;
}

/**
 * @brief Searches the vector for all instances of n.
 *
 * @param v Vector to search.
 * @param n Number to search for.
 * @return A vector of indices where n was found.
 */
std::vector<int>
OpsVector::SearchVector(std::vector<int> &v, int n) {
  std::vector<int> ret;
  ret.reserve(v.size());
  for (int i = 0, sz = (int)v.size(); i < sz; ++i) {
    if (v[i] == n) {
      ret.push_back(i);
    }
  }
  ret.shrink_to_fit();
  return ret;
}

/**
 * @brief Sorts the vector in ascending order.
 *
 * @param v Vector to sort.
 * @return The sorted vector.
 */
std::vector<int>
OpsVector::SortVector(std::vector<int> &v) {
  std::vector<int> ret(v);
  std::sort(ret.begin(), ret.end());
  return ret;
}

/**
 * @brief Reverses the vector.
 *
 * @param v Vector to reverse.
 * @return The reversed vector.
 */
std::vector<int>
OpsVector::ReverseVector(std::vector<int> &v) {
  std::vector<int> ret(v);
  std::reverse(ret.begin(), ret.end());
  return ret;
}

/**
 * @brief Rotates the vector by n.
 *
 * @param v Vector to rotate.
 * @param n Number of elements to rotate by.
 * @return The rotated vector.
 */
std::vector<int>
OpsVector::RotateVector(std::vector<int> &v, int n) {
  std::vector<int> ret(v.size());
  int sz = (int)v.size();
  if (sz == 0) return ret;
  n = ((n % sz) + sz) % sz; // Ensure n is in [0, sz)
  std::rotate_copy(v.begin(), v.begin() + n, v.end(), ret.begin());
  return ret;
}

/**
 * @brief Merges two vectors.
 *
 * @param v1 First vector.
 * @param v2 Second vector.
 * @return The merged vector.
 */
std::vector<int>
OpsVector::MergeVectors(std::vector<int> &v1, std::vector<int> &v2) {
  std::vector<int> ret;
  ret.reserve(v1.size() + v2.size());
  ret.insert(ret.end(), v1.begin(), v1.end());
  ret.insert(ret.end(), v2.begin(), v2.end());
  return ret;
}