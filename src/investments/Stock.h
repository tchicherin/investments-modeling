#pragma once
// Stock.h
// Simplified stock instrument: value tied to market stock index and volatility.

#ifndef STOCK_H
#define STOCK_H

#include "../Investment.h"
#include "../Market.h"

class Stock : public Investment {
 public:
  explicit Stock(const std::string& ticker, double volatility = 0.2);

  void UpdateFrame(const MarketSnapshot& market_snapshot, int step_index) override;

  [[nodiscard]] double ComputeProfit(const MarketSnapshot& market_snapshot, int step_index) const override;

  [[nodiscard]] std::string TypeName() const override { return "Stock"; }

  [[nodiscard]] const std::string& Ticker() const { return ticker_; }

 private:
  std::string ticker_;
  // Instrument-specific state can be added here (e.g. last known price)
};

#endif // STOCK_H
