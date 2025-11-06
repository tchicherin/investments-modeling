#include "StockInvestment.h"
#include "Market.h"

#include <algorithm>

StockInvestment::StockInvestment(const QString &name, const QString &assetName, double amount, const Market &m)
  : Investment(name), assetName_(assetName) {
  double price = 1.0;
  for (const auto &c : m.companies())
    if (c.name == assetName_) { price = c.price; break; }
  shares_ = amount / price;
  lastPrice_ = price;
}

double StockInvestment::marketValue(const Market &m) const {
  double price = 1.0;
  for (const auto &c : m.companies())
    if (c.name == assetName_) { price = c.price; break; }
  return shares_ * price;
}

double StockInvestment::stepMonth(const Market &m) {
  double newPrice = 1.0;
  for (const auto &c : m.companies())
    if (c.name == assetName_) { newPrice = c.price; break; }
  double oldValue = shares_ * lastPrice_;
  double newValue = shares_ * newPrice;
  lastPrice_ = newPrice;
  return newValue - oldValue;
}
