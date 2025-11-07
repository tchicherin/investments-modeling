#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <mutex>

// Rng — единый генератор, инициализируемый извне.
// Если не вызвать init(), будет использован random_device-seeded генератор.
struct Rng {
  static void init(unsigned int seed) {
    std::call_once(init_flag_, [&]() {
      g_.seed(seed);
    });
  }

  static std::mt19937 &gen() {
    return g_;
  }

  static double uniform(double a, double b) {
    std::uniform_real_distribution<double> d(a, b);
    return d(gen());
  }

  static double normal(double mean, double stddev) {
    std::normal_distribution<double> d(mean, stddev);
    return d(gen());
  }

private:
  static std::mt19937 g_;
  static std::once_flag init_flag_;
};

#endif // UTILS_H
