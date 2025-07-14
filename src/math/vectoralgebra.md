std::vector<double>
MathVectorAlgebra::Cubic(int a, int b, int c, int d, const std::vector<double> &x) {
  auto n = x.size();
  std::vector<double> y(n);

  sycl::queue q(sycl::default_selector{});
  sycl::buffer<double, 1> y_buf(y.data(), sycl::range<1>(n));

  q.submit([&](sycl::handler &h) {
    auto y_acc = y_buf.get_access<sycl::access::mode::discard_write>(h);
    h.parallel_for(sycl::range<1>(n), [=](sycl::id<1> i) {
      double xi = x[i];
      y_acc[i] = a * xi * xi * xi + b * xi * xi + c * xi + d;
    });
  }).wait();

  return y;
}