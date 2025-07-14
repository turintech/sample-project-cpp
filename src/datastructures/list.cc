#include "list.h"
#include <vector>
#include <algorithm>
#include <random>

/** @brief shuffles a list into a new list
 *
 * @param l the list to shuffle
 * @return a new list with the elements of l shuffled
 */
std::list<int>
OpsList::Shuffle(std::list<int> &l) {
  // Use std::vector for efficient shuffling and reduced memory moves.
  std::vector<int> v(l.begin(), l.end());
  // Use a more robust random device, not srand/rand
  std::mt19937 rng(0); // deterministic for testing
  std::shuffle(v.begin(), v.end(), rng);
  return std::list<int>(v.begin(), v.end());
}

/** @brief slices a list into a new list
 *
 * @param l the list to slice
 * @param start the start index of the slice
 * @param end the end index of the slice (exclusive)
 * @return a new list with the elements of l sliced
 */
std::list<int>
OpsList::Slice(std::list<int> &l, int start, int end) {
  std::list<int> ret;
  // guard against out-of-bounds
  if (start < 0) start = 0;
  if (end > (int)l.size()) end = l.size();
  if (start >= end) return ret;

  std::list<int>::iterator it = l.begin();
  // Only advance once for start, then copy using single pass
  std::advance(it, start);
  for (int i = start; i < end; ++i) {
    ret.push_back(*it);
    ++it;
  }
  return ret;
}