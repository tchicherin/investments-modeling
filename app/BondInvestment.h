#ifndef BOND_INVESTMENT_H
#define BOND_INVESTMENT_H

#include "Investment.h"

class BondInvestment final : public Investment {
  double principal_;
  double annualYield_;
public:
  BondInvestment(const QString &name, double amount, double annualYield);

  [[nodiscard]] QString type() const override { return "Bond"; }
  [[nodiscard]] double marketValue(const Market &m) const override;
  double stepMonth(const Market &m) override;
};

#endif // BOND_INVESTMENT_H
