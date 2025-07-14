# SORT

## SortVector

This sorts an integer vector using `std::sort`. It's quite optimal
already. 

- Use `std::sort` (for serial) or `std::stable_sort(std::execution::par, ...)` for parallelization to utilize multiple cores and reduce runtime on large inputs.

## DutchFlagPartition

Partitions a vector around some pivot point. 

- Replace multi-pass implementations with a single for loop using 3-way partitioning (Dijkstra's Dutch National Flag algorithm), reducing runtime and memory usage by avoiding auxiliary arrays.

## MaxN

Returns the maximum N values of a vector, relying on `std::sort` initially.

- Use `std::partial_sort` to find the top N values with better performance and lower memory usage than a full sort.
- For very large vectors or in parallel settings, consider `std::nth_element` to partition the largest N values, then sort only those N elements if ordering is needed.