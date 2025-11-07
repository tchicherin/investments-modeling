#include "BondInvestment.h"
#include "Market.h"

BondInvestment::BondInvestment(const QString &name, double amount, double annualYield)
  : Investment(name), principal_(amount), annualYield_(annualYield) {}

double BondInvestment::marketValue(const Market &) const {
  return principal_;
}

double BondInvestment::stepMonth(const Market &) {
  double monthlyProfit = principal_ * (annualYield_ / 12.0);
  principal_ += monthlyProfit;
  return monthlyProfit;
}
