#pragma once
#include <random>

struct Rng {
    static std::mt19937& gen() {
        static std::random_device rd;
        static std::mt19937 g(rd());
        return g;
    }
    static double uniform(double a, double b) {
        std::uniform_real_distribution<double> d(a, b);
        return d(gen());
    }
    static double normal(double mean, double stddev) {
        std::normal_distribution<double> d(mean, stddev);
        return d(gen());
    }
};
