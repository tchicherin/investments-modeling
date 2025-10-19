#pragma once
// Simulator.h
// Coordinates Market, Fund and the simulation loop (step by step or full run).

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Market.h"
#include "Fund.h"

struct SimulatorConfig {
  int steps = 12;          // number of steps to simulate (e.g. months)
  int start_step = 0;
  uint32_t rng_seed = 0;
  bool seed_provided = false;
};

class Simulator {
 public:
  explicit Simulator(const SimulatorConfig& cfg);
  ~Simulator() = default;

  // Run all steps (blocking)
  void Run();

  // Advance single simulation step (useful for UI step-by-step)
  void Step();

  // reset uses stored config to restart
  void Reset();

  [[nodiscard]] const Fund& GetFund() const { return fund_; }
  [[nodiscard]] const Market& GetMarket() const { return market_; }

 private:
  SimulatorConfig cfg_;
  Market market_;
  Fund fund_;
  int current_step_;
  bool running_;
};

#endif // SIMULATOR_H
