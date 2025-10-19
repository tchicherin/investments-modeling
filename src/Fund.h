#pragma once
// Fund.h
// High-level wrapper containing a Portfolio and fund-level parameters (tax, inflows/outflows).

#ifndef FUND_H
#define FUND_H

#include "Portfolio.h"

class Fund {
 public:
  explicit Fund(double initial_capital = 0.0, double tax_rate = 0.0);

  void SetPortfolio(std::unique_ptr<Portfolio> p) { portfolio_ = std::move(p); }

  // Called each simulation step to apply portfolio updates and fund-level rules
  void UpdateFrame(const MarketSnapshot& market_snapshot, int step_index);

  // External cash flows
  void AddInflows(double amount) { capital_ += amount; }
  void AddOutflows(double amount) { capital_ -= amount; }

  [[nodiscard]] double GetCapital() const { return capital_; }
  [[nodiscard]] double GetTaxRate() const { return tax_rate_; }

 private:
  double capital_{0.0};
  double tax_rate_{0.0};

  std::unique_ptr<Portfolio> portfolio_;
};

#endif // FUND_H
