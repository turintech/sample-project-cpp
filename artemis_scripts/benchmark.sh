#!/bin/bash

# Import variables
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source "$DIR/variables.sh"

# Run only the SumPrimes benchmark case; the artemis-json reporter also
# filters to SumPrimes*_mean_ns so artemis_results.json stays focused.
BENCHMARK='build/tests/tests "Primes::SumPrimes::Benchmark"'
echo "Running benchmark command: $BENCHMARK"
eval $BENCHMARK

echo "Artemis results written to artemis_results.json"
