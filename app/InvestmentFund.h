// вставь в InvestmentFund.h (замени или расширь существующий файл)
#ifndef INVESTMENTFUND_H
#define INVESTMENTFUND_H

#include "Portfolio.h"
#include <QString>
#include <vector>

class Market;

struct MonthResult {
  int monthIndex = 0;
  double equityBefore = 0.0;
  double equityAfter = 0.0;
  double portfolioProfit = 0.0; // до налога
  double taxPaid = 0.0;
  double netProfit = 0.0; // после налога и учета flows
  double flow = 0.0;      // притоки (+) или оттоки (-)
  double cashBefore = 0.0;
  double cashAfter = 0.0;

  // детализация по типам (по желанию заполнять)
  double profitDeposits = 0.0;
  double profitStocks = 0.0;
  double profitBonds = 0.0;
  double profitCurrency = 0.0;
  double profitMetals = 0.0;
};

class InvestmentFund {
  double cash_ = 0.0; // свободные деньги
  double totalProfit_ = 0.0; // накопленная прибыль
  Portfolio portfolio_;

 public:
  explicit InvestmentFund(double initialCapital);

  Portfolio &portfolio() { return portfolio_; }
  [[nodiscard]] const Portfolio &portfolio() const { return portfolio_; }

  [[nodiscard]] double cash() const { return cash_; }

  [[nodiscard]] double equity(const Market &m) const; // cash + стоимость портфеля
  [[nodiscard]] double totalProfit() const { return totalProfit_; }

  // Симуляция месяца — заполняет MonthResult (включая налог и притоки)
  MonthResult simulateMonthDetailed(Market &market, double taxRate, int monthIndex);

  // старый метод можно оставить (для обратной совместимости) или удалить
  double simulateMonth(Market &market, double taxRate);

  // Операции
  bool buyDeposit(const QString &name, double amount, double annualRate, int months);
  bool buyStock(const QString &name, double amount, const Market &m);
  bool buyCurrency(const QString &name, double amount, double monthlyRate); // пример
  bool buyBond(const QString &name, double amount, double annualYield);
  bool buyMetal(const QString &name, double amount, const Market &m);

  void sellInvestment(int index, const Market &m);

  // низкоуровневые операции для UI (сеттеры)
  void setCash(double c) { cash_ = c; }
};
#endif // INVESTMENTFUND_H