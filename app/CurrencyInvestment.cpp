#include "CurrencyInvestment.h"
#include "Market.h"

CurrencyInvestment::CurrencyInvestment(const QString &name, double amount, double monthlyRate)
  : Investment(name), amount_(amount), rate_(monthlyRate) {}

double CurrencyInvestment::marketValue(const Market &) const {
  return amount_;
}

double CurrencyInvestment::stepMonth(const Market &) {
  double profit = amount_ * rate_;
  amount_ += profit;
  return profit;
}
