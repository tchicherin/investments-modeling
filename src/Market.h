#pragma once
// Market.h
// Defines MarketSnapshot and Market class that simulates external market parameters.

#ifndef MARKET_H
#define MARKET_H

#include "RandomEngine.h"

struct MarketSnapshot {
  // Interpret these as monthly rates / prices (document in README)
  double deposit_rate_month = 0.0; // e.g. 0.005 -> 0.5% per month
  double bond_yield_month = 0.0;
  double stock_index_price = 1000.0; // abstract price index
  double gold_price = 100.0;
  double volatility = 0.0; // aggregate volatility parameter
};

class Market {
 public:
  explicit Market(uint32_t seed = 0);
  // Advance market by one simulation step (e.g. one day or one month depending on config)
  void UpdateFrame(int step_index);

  [[nodiscard]] const MarketSnapshot& GetSnapshot() const { return snapshot_; }

  // configuration setters/getters
  void SetBaseDepositRateAnnual(double annual_rate);
  void SetVolatility(double vol);

 private:
  MarketSnapshot snapshot_;

  // internal parameters
  double base_deposit_rate_annual_ = 0.0;
  double vol_ = 0.1;

  void RandomWalkPrices(int step_index);
};

#endif // MARKET_H
