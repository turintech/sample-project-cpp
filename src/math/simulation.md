using namespace sycl;
std::vector<double>
MathSimulation::SimpleGravity(const std::vector<double> &positions, double dt, int iterations) {
  if (positions.size() % 3 != 0) {
    throw std::invalid_argument("Invalid matrix size `positions`, expected a multiple of 3");
  }
  int n = positions.size() / 3;
  std::vector<double> result(positions);

  queue q(default_selector_v);
  buffer<double, 1> positionsBuffer(result.data(), range<1>(3 * n));
  buffer<double, 1> forcesBuffer(range<1>(3 * n));

  for (int iter = 0; iter < iterations; ++iter) {
    q.submit([&](handler &h) {
      auto forces = forcesBuffer.get_access<access::mode::discard_write>(h);
      h.parallel_for<class resetForcesKernel>(range<1>(3 * n), [=](id<1> i) { forces[i] = 0.0; });
    });

    q.submit([&](handler &h) {
      auto pos = positionsBuffer.get_access<access::mode::read>(h);
      auto forces = forcesBuffer.get_access<access::mode::atomic>(h);
      h.parallel_for<class NBodyKernel>(range<1>(n * (n - 1) / 2), [=](id<1> idx) {
        int k = idx[0];
        int i = static_cast<int>((sqrt(8.0 * k + 1) - 1) * 0.5);
        int j = k - i * (i + 1) / 2 + i + 1;
        if (i < n && j < n && i < j) {
          double dx = pos[j * 3 + 0] - pos[i * 3 + 0];
          double dy = pos[j * 3 + 1] - pos[i * 3 + 1];
          double dz = pos[j * 3 + 2] - pos[i * 3 + 2];
          double dist = sqrt(dx * dx + dy * dy + dz * dz) + 1e-10;
          double forceMagnitude = G / (dist * dist);
          double fx = forceMagnitude * dx / dist;
          double fy = forceMagnitude * dy / dist;
          double fz = forceMagnitude * dz / dist;
          auto& fi0 = forces[i * 3 + 0]; fi0.fetch_add(fx);
          auto& fi1 = forces[i * 3 + 1]; fi1.fetch_add(fy);
          auto& fi2 = forces[i * 3 + 2]; fi2.fetch_add(fz);
          auto& fj0 = forces[j * 3 + 0]; fj0.fetch_add(-fx);
          auto& fj1 = forces[j * 3 + 1]; fj1.fetch_add(-fy);
          auto& fj2 = forces[j * 3 + 2]; fj2.fetch_add(-fz);
        }
      });
    });

    q.submit([&](handler &h) {
      auto pos = positionsBuffer.get_access<access::mode::read_write>(h);
      auto forces = forcesBuffer.get_access<access::mode::read>(h);
      h.parallel_for<class updatePositionsKernel>(range<1>(n), [=](id<1> i) {
        pos[i * 3 + 0] += dt * forces[i * 3 + 0];
        pos[i * 3 + 1] += dt * forces[i * 3 + 1];
        pos[i * 3 + 2] += dt * forces[i * 3 + 2];
      });
    });
  }

  q.submit([&](handler &h) {
    auto pos = positionsBuffer.get_access<access::mode::read>(h);
    h.copy(pos, result.data());
  }).wait();

  return result;
}