#include "MetalInvestment.h"
#include "Market.h"
#include <algorithm>

MetalInvestment::MetalInvestment(const QString &name, double amount, double initialPrice)
    : Investment(name), amount_(amount), lastPrice_(initialPrice) {}

double MetalInvestment::marketValue(const Market &m) const {
  double price = 60.0;
  for (const auto &mInfo : m.metals()) {
    if (mInfo.name == name_) {
      price = mInfo.price;
      break;
    }
  }
  return amount_ * price;
}

double MetalInvestment::stepMonth(const Market &m) {
  double price = 60.0;
  for (const auto &mInfo : m.metals()) {
    if (mInfo.name == name_) {
      price = mInfo.price;
      break;
    }
  }
  double value = amount_ * price;
  double profit = value - amount_ * lastPrice_;
  lastPrice_ = price;
  return profit;
}