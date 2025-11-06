#ifndef INVESTMENTFUND_H
#define INVESTMENTFUND_H

#include "Portfolio.h"
#include <QString>

class Market;

class InvestmentFund {
  double cash_ = 0.0; // свободные деньги
  double totalProfit_ = 0.0; // накопленная прибыль
  Portfolio portfolio_;

public:
  explicit InvestmentFund(double initialCapital);

  Portfolio &portfolio() { return portfolio_; }
  [[nodiscard]] const Portfolio &portfolio() const { return portfolio_; }

  [[nodiscard]] double cash() const { return cash_; }
  void setCash(double c) { cash_ = c; } // setter для UI

  [[nodiscard]] double equity(const Market &m) const; // cash + стоимость портфеля
  [[nodiscard]] double totalProfit() const { return totalProfit_; }

  // Симуляция месяца: вернуть прибыль ДО налога (для UI)
  double simulateMonth(Market &market, double taxRate);

  // Операции
  bool buyDeposit(const QString &name, double amount, double annualRate, int months);

  bool buyStock(const QString &name, double amount, const Market &m);

  void sellInvestment(int index, const Market &m);
};

#endif // INVESTMENTFUND_H