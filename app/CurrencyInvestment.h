#ifndef CURRENCY_INVESTMENT_H
#define CURRENCY_INVESTMENT_H

#include "Investment.h"

class CurrencyInvestment final : public Investment {
  double amount_;    // сумма в валюте
  double rate_;      // месячная доходность (например, 0.01 = 1%)
 public:
  CurrencyInvestment(const QString &name, double amount, double monthlyRate);

  [[nodiscard]] QString type() const override { return "Currency"; }
  [[nodiscard]] double marketValue(const Market &m) const override;
  double stepMonth(const Market &m) override;
};

#endif // CURRENCY_INVESTMENT_H
