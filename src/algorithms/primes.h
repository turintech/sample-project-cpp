#ifndef ALGORITHMS_PRIMES_H_
#define ALGORITHMS_PRIMES_H_

#include <vector>

namespace Primes {

// Returns the sum of all primes less than or equal to n.
// Optimized using Sieve of Eratosthenes for better performance and memory usage.
int SumPrimes(int n);

// Returns the prime factors of n in ascending order.
// Improved to avoid unnecessary copies and optimizes prime check by limiting to sqrt(n).
std::vector<int> PrimeFactors(int n);

// Checks if n is a prime number using sqrt(n) optimization.
// Changed signature to take n by value (usually an int, so no overhead).
bool IsPrime(int n);

};   // namespace Primes

#endif   // ALGORITHMS_PRIMES_H_