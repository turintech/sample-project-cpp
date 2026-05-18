#!/bin/bash

# Import variables
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source "$DIR/variables.sh"

# The test binary's custom main writes serialised benchmark results to
# artemis_results.json by default (alongside console output).
BENCHMARK="build/tests/tests [benchmark]"
echo "Running benchmark command: $BENCHMARK"
eval $BENCHMARK

echo "Artemis results written to artemis_results.json"
