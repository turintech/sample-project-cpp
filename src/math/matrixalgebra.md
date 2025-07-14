## Notes

For high performance substitution, 1d matrices are preferred. They work with mkl and sycl directly.
Avoid converting between 2d and 1d matrix representations at runtime. Store matrices as contiguous 1d arrays (`std::vector<double>`) whenever possible to reduce memory usage and enable direct use with MKL/SYCL backends.
We don't opt for an interface since this would need to be provided to the LLM. In the future, we'd
want a way to refactor from 2d vector implementation to 1d implementation but this is tricky.