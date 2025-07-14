#include "sort.h"

#include <algorithm>
#include <queue>

/**
 * @brief Sorts a vector of integers (in place)
 *
 * @param v the vector to sort
 */
void
Sort::SortVector(std::vector<int> &v) {
  std::sort(v.begin(), v.end());
}

/**
 * @brief Partitions a vector of integers around a pivot
 *
 * @param v the vector to partition
 * @param pivot_value value of the pivot
 */
void
Sort::DutchFlagPartition(std::vector<int> &v, int pivot_value) {
  int smaller = 0, equal = 0, larger = (int) v.size();
  // In-place O(n) three-way partitioning (more efficient for large arrays, less cache churn)
  while (equal < larger) {
    if (v[equal] < pivot_value) {
      std::swap(v[smaller++], v[equal++]);
    } else if (v[equal] == pivot_value) {
      ++equal;
    } else {
      std::swap(v[equal], v[--larger]);
    }
  }
}

/**
 * @brief Returns the largest n elements in a vector
 *
 * @param v the vector to search
 * @param n the number of elements to return
 * @return the largest n elements in the vector
 *
 * This implementation uses a min-heap to keep only n largest items,
 * reducing memory usage and improving runtime for large inputs.
 */
std::vector<int>
Sort::MaxN(std::vector<int> &v, int n) {
  // Edge cases
  if (n <= 0)
    return {};
  if ((int)v.size() <= n) {
    std::vector<int> ret(v);
    std::sort(ret.begin(), ret.end(), std::greater<int>());
    return ret;
  }

  // Min-heap to keep track of n largest elements
  std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

  for (int x : v) {
    if ((int)minHeap.size() < n) {
      minHeap.push(x);
    } else if (x > minHeap.top()) {
      minHeap.pop();
      minHeap.push(x);
    }
  }
  std::vector<int> ret;
  ret.reserve(n);
  while (!minHeap.empty()) {
    ret.push_back(minHeap.top());
    minHeap.pop();
  }
  std::sort(ret.begin(), ret.end(), std::greater<int>());
  return ret;
}