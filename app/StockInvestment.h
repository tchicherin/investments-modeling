#ifndef STOCK_INVESTMENT_H
#define STOCK_INVESTMENT_H

#include "Investment.h"

class StockInvestment : public Investment {
  double shares_; // количество купленных «акций» индекса
  double lastPrice_; // последняя цена для расчёта Δ

 public:
  // amount инвестируется по текущей цене — считаем количество shares
  StockInvestment(const QString &name, double amount, const Market &m);

  [[nodiscard]] QString type() const override { return "Stock"; }

  [[nodiscard]] double marketValue(const Market &m) const override;

  double stepMonth(const Market &m) override;
};

#endif // STOCK_INVESTMENT_H
