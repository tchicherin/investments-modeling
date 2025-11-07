#ifndef DEPOSIT_INVESTMENT_H
#define DEPOSIT_INVESTMENT_H

#include "Investment.h"

class DepositInvestment final : public Investment {
  double principal_;     // тело депозита
  double annualRate_;    // годовая ставка, 0.08 = 8%
  int monthsLeft_;       // месяцев до пересмотра/пролонгации
  int termMonths_;       // исходный срок для цикличной пролонгации

 public:
  DepositInvestment(const QString& name, double amount, double annualRate, int months);

  [[nodiscard]] QString type() const override { return "Deposit"; }
  [[nodiscard]] double marketValue(const Market& m) const override;
  double stepMonth(const Market& m) override;
};

#endif // DEPOSIT_INVESTMENT_H
