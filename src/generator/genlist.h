#ifndef __GENERATOR_GENLIST_H__
#define __GENERATOR_GENLIST_H__

#include <vector>
#include <list>

namespace GenList {

/**
 * Generates a list of 'n' random integers in the range [0, m).
 * To improve memory usage and provide better cache locality, consider:
 * - Generating the random numbers in a preallocated std::vector first, 
 *   then transferring to std::list if required (not applicable here as only the interface changes).
 * - If random access or contiguous storage is desired, use std::vector directly.
 * Since <list> is used here, but only the interface is defined, 
 * no memory or runtime overhead can be optimized at this level.
 */
std::list<int> RandomList(int n, int m);

}  // namespace GenList

#endif   // __GENERATOR_GENLIST_H__