# PRIMES

## IsPrime

Returns true if the value is prime. 

- Reduce the max value of the loop to the sqrt of `n` to improve runtime and reduce unnecessary iterations.

## SumPrimes

Sums the primes values up to `n`. Calls `IsPrime` for each
value. This is a very inefficient solution.

- Use the Sieve of Eratosthenes to compute all primes up to `n`, then sum them. This reduces both runtime and memory usage compared to individually checking each number for primality.

## PrimeFactors

Computes prime factors of n.

- Precompute primes up to sqrt(n) using a sieve and use them for factorization to improve both speed and memory efficiency.