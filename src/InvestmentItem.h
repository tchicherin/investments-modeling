#pragma once
// InvestmentItem.h
// Represents a holding (position) in a specific Investment instrument.
// By default works as a bank account-like wrapper for monetary amount.

#ifndef INVESTMENTITEM_H
#define INVESTMENTITEM_H

#include <stdexcept>
#include <memory>
#include <string>
#include <vector>

#include "Investment.h"

// A small transaction record
struct Transaction {
  int step_index;
  double amount;   // positive = buy/inflow, negative = sell/outflow
  std::string note;
};

class InvestmentItem {
 public:
  // base — shared ownership of Investment definition (instrument behavior)
  InvestmentItem(std::shared_ptr<const Investment> base, double money, std::string id = {})
      : base_(std::move(base)), total_money_(money), available_money_(money), id_(std::move(id)) {}

  virtual ~InvestmentItem() = default;

  [[nodiscard]] virtual double GetTotalMoney() const { return total_money_; }
  [[nodiscard]] virtual double GetAvailableMoney() const { return available_money_; }

  // Buy adds funds to this item (e.g. purchase)
  virtual void Buy(double amount) {
    total_money_ += amount;
    available_money_ += amount;
    history_.push_back({-1, amount, "Buy"});
  }

  // Sell withdraws funds; by default checks available amount
  virtual void Sell(double amount) {
    if (amount > available_money_) {
      throw std::invalid_argument("Attempt to sell an amount of money exceeding the amount of available money");
    }
    total_money_ -= amount;
    available_money_ -= amount;
    history_.push_back({-1, -amount, "Sell"});
  }

  // Called each simulation step to let the item update (accrue interest, value changes)
  virtual void UpdateFrame(const MarketSnapshot& snapshot, int step_index) {
    // default: do nothing; derived classes may use ComputeProfit and update balances
    (void)snapshot;
    (void)step_index;
  }

  [[nodiscard]] std::shared_ptr<const Investment> GetInstrument() const { return base_; }
  [[nodiscard]] const std::string& GetId() const { return id_; }

  [[nodiscard]] const std::vector<Transaction>& GetHistory() const { return history_; }

 protected:
  std::shared_ptr<const Investment> base_;
  double total_money_{0.0}, available_money_{0.0};
  std::string id_;
  std::vector<Transaction> history_;
};

#endif // INVESTMENTITEM_H
