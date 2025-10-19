#pragma once
// Portfolio.h
// Collection of InvestmentItem positions.

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "InvestmentItem.h"
#include "Market.h"

class Portfolio {
 public:
  Portfolio() = default;
  ~Portfolio() = default;

  void AddItem(std::unique_ptr<InvestmentItem> item) {
    items_.push_back(std::move(item));
  }

  // remove by id (returns true if removed)
  bool RemoveItemById(const std::string& id) {
    auto it = std::remove_if(items_.begin(), items_.end(),
                             [&](const std::unique_ptr<InvestmentItem>& p) { return p->GetId() == id; });
    if (it == items_.end()) return false;
    items_.erase(it, items_.end());
    return true;
  }

  [[nodiscard]] double GetTotalCapital() const {
    double sum = 0.0;
    for (const auto& it : items_) sum += it->GetTotalMoney();
    return sum;
  }

  [[nodiscard]] double GetAvailableCapital() const {
    double sum = 0.0;
    for (const auto& it : items_) sum += it->GetAvailableMoney();
    return sum;
  }

  // Update all items for one simulation step
  void UpdateFrame(const MarketSnapshot& market_snapshot, int step_index) {
    for (auto& it : items_) it->UpdateFrame(market_snapshot, step_index);
  }

  [[nodiscard]] const std::vector<std::unique_ptr<InvestmentItem>>& Items() const { return items_; }

 private:
  std::vector<std::unique_ptr<InvestmentItem>> items_;
};

#endif // PORTFOLIO_H
