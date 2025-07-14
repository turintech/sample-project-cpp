#ifndef TESTHELPERS_TESTERVECTOR_H_
#define TESTHELPERS_TESTERVECTOR_H_

#include <cstddef>
#include <cstdlib>
#include <vector>

namespace TestHelpers {

// Fully inlined info to reduce temporary output string construction
template <typename T>
void
check_vectors_equal(const std::vector<T> &v1, const std::vector<T> &v2) {
  const size_t n = v1.size();
  INFO("Comparing vector length: Expected " << n << ", got " << v2.size());
  REQUIRE(n == v2.size());
  for (size_t i = 0; i < n; ++i) {
    const T &expected = v1[i];
    const T &got = v2[i];
    INFO("Comparing element at index " << i << ": Expected " << expected << ", got " << got);
    REQUIRE(expected == got);
  }
}

template <typename T>
void
check_vectors_almost_equal(const std::vector<T> &v1, const std::vector<T> &v2, double absdiff = 0.02) {
  const size_t n = v1.size();
  INFO("Comparing vector length: Expected " << n << ", got " << v2.size());
  REQUIRE(n == v2.size());
  for (size_t i = 0; i < n; ++i) {
    const T &expected = v1[i];
    const T &got = v2[i];
    INFO("Comparing element at index " << i << ": Expected " << expected << ", got " << got);
    REQUIRE(std::abs(expected - got) < absdiff);
  }
}

template <typename T>
void
check_matrices_equal(const std::vector<std::vector<T>> &a1, const std::vector<std::vector<T>> &a2) {
  const size_t rows = a1.size();
  INFO("Comparing matrix shape: Expected (" << rows << ", " 
                                            << (rows > 0 ? a1[0].size() : 0) << ")"
                                            << ", got (" << a2.size() << ", "
                                            << (a2.size() > 0 ? a2[0].size() : 0) << ")");
  REQUIRE(rows == a2.size());
  if (rows > 0 || a2.size() > 0) {
    REQUIRE(a1[0].size() == a2[0].size());
  }
  const size_t cols = (rows > 0 ? a1[0].size() : 0);

  for (size_t i = 0; i < rows; ++i) {
    const std::vector<T> &row1 = a1[i];
    const std::vector<T> &row2 = a2[i];
    for (size_t j = 0; j < cols; ++j) {
      INFO("Comparing element at index (" << i << ", " << j << "): Expected " << row1[j] << ", got " << row2[j]);
      REQUIRE(row1[j] == row2[j]);
    }
  }
}

template <typename T>
void
check_matrices_almost_equal(const std::vector<std::vector<T>> &a1, const std::vector<std::vector<T>> &a2,
                            double absdiff = 0.02) {
  const size_t rows = a1.size();
  INFO("Comparing matrix shape: Expected (" << rows << ", "
                                            << (rows > 0 ? a1[0].size() : 0) << ")"
                                            << ", got (" << a2.size() << ", "
                                            << (a2.size() > 0 ? a2[0].size() : 0) << ")");
  REQUIRE(rows == a2.size());
  if (rows > 0 || a2.size() > 0) {
    REQUIRE(a1[0].size() == a2[0].size());
  }
  const size_t cols = (rows > 0 ? a1[0].size() : 0);

  for (size_t i = 0; i < rows; ++i) {
    const std::vector<T> &row1 = a1[i];
    const std::vector<T> &row2 = a2[i];
    for (size_t j = 0; j < cols; ++j) {
      INFO("Comparing element at index (" << i << ", " << j << "): Expected " << row1[j] << ", got " << row2[j]);
      REQUIRE(std::abs(row1[j] - row2[j]) < absdiff);
    }
  }
}

}   // namespace TestHelpers

#endif   // !TESTHELPERS_TESTERVECTOR_H_