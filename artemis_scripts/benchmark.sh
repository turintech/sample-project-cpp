#!/bin/bash

# Import variables
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source "$DIR/variables.sh"

# Output file for serialised benchmark results (Catch2 XML reporter).
# Contains per-benchmark mean, standard deviation, sample count, etc.
RESULTS_FILE="artemis_results.xml"

# Run benchmarks with two reporters: console (stdout) and XML (to file).
BENCHMARK="build/tests/tests [benchmark] --reporter console::out=- --reporter XML::out=${RESULTS_FILE}"
echo "Running benchmark command: $BENCHMARK"
eval $BENCHMARK

echo "Artemis results written to ${RESULTS_FILE}"
