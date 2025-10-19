#pragma once
// Investment.h
// Abstract base class for investment instruments.

#ifndef IINVESTMENT_H
#define IINVESTMENT_H

#include <optional>
#include <string>

struct MarketSnapshot; // forward (defined in Market.h)

class Investment {
 public:
  virtual ~Investment() = default;

  // Called each simulation step to update internal state (prices, coupon accrual etc.)
  virtual void UpdateFrame(const MarketSnapshot& market_snapshot, int step_index) = 0;

  // Compute profit (positive or negative) for the current step (same time unit as UpdateFrame)
  [[nodiscard]] virtual double ComputeProfit(const MarketSnapshot& market_snapshot, int step_index) const = 0;

  virtual void SetInterestRate(double value) { interest_rate_ = value; }
  [[nodiscard]] virtual double GetInterestRate() const { return interest_rate_; }

  [[nodiscard]] virtual double GetVolatility() const { return volatility_; }

  [[nodiscard]] virtual bool HasTerm() const { return term_.has_value(); }
  [[nodiscard]] virtual int GetTerm() const { return *term_; }

  [[nodiscard]] virtual std::string TypeName() const = 0;

 protected:
  // Protected constructor for derived classes
  explicit Investment(double volatility = 0.0, std::optional<int> term = std::nullopt)
      : interest_rate_(0.0), volatility_(volatility), term_(term) {}

  double interest_rate_;
  double volatility_;
  std::optional<int> term_;
};

#endif // IINVESTMENT_H
