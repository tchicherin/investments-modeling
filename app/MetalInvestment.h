#ifndef METAL_INVESTMENT_H
#define METAL_INVESTMENT_H

#include "Investment.h"

class Market;

class MetalInvestment final : public Investment {
  double amount_;
  double lastPrice_; // для вычисления Δ

public:
  // Конструктор: передаем имя, количество и начальную цену металла
  MetalInvestment(const QString &name, double amount, double initialPrice);

  [[nodiscard]] QString type() const override { return "Metal"; }

  [[nodiscard]] double marketValue(const Market &m) const override;

  double stepMonth(const Market &m) override;
};

#endif // METAL_INVESTMENT_H