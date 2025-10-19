#pragma once
// RandomEngine.h
// Singleton wrapper for RNG used across the application.

#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

#include <random>
#include <cstdint>

class RandomEngine {
 public:
  RandomEngine(const RandomEngine&) = delete;
  RandomEngine& operator=(const RandomEngine&) = delete;

  static RandomEngine& Instance() {
    static RandomEngine instance;
    return instance;
  }

  std::mt19937& Get() { return engine_; }

  void Seed(uint32_t s) { engine_.seed(s); }

 private:
  RandomEngine() : engine_(std::random_device{}()) {}
  ~RandomEngine() = default;

  std::mt19937 engine_;
};

#endif // RANDOMENGINE_H
