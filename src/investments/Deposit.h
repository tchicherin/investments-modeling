#pragma once
// Deposit.h
// Simple deposit instrument (fixed interest, optional term).

#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "../Investment.h"
#include "../Market.h"

class Deposit : public Investment {
public:
  // term_months: optional maturity term in months (0/empty = no lock)
  explicit Deposit(double annual_rate, std::optional<int> term_months = std::nullopt, double volatility = 0.0);

  void UpdateFrame(const MarketSnapshot& market_snapshot, int step_index) override;

  [[nodiscard]] double ComputeProfit(const MarketSnapshot& market_snapshot, int step_index) const override;

  [[nodiscard]] std::string TypeName() const override { return "Deposit"; }

private:
  // stored as annual nominal rate; conversion to monthly done in implementation
  int term_months_;
  // any other stateful fields can be added (accrued interest etc.)
};

#endif // DEPOSIT_H
