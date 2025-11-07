#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include "InvestmentFund.h"
#include "Market.h"

class GameController : public QObject {
  Q_OBJECT
  Market market_;
  InvestmentFund fund_;
  int month_ = 0;
  int totalMonths_;
  double taxRate_; // 0.17 = 17%

  double lastMonthProfit_ = 0.0;

public:
  explicit GameController(double initialCapital, int totalMonths, double taxRate, unsigned int seed, QObject *parent = nullptr);

  [[nodiscard]] const Market &market() const { return market_; }
  Market &market() { return market_; }

  [[nodiscard]] const InvestmentFund &fund() const { return fund_; }
  InvestmentFund &fund() { return fund_; }

  [[nodiscard]] int month() const { return month_; }
  [[nodiscard]] int totalMonths() const { return totalMonths_; }
  [[nodiscard]] double lastMonthProfit() const { return lastMonthProfit_; }
  [[nodiscard]] double taxRate() const { return taxRate_; }

  // setters for runtime changes from UI
  void setTaxRate(double r) { taxRate_ = r; }
  void setTotalMonths(int m) { totalMonths_ = m; }
  void resetMonthCounter() { month_ = 0; }

public slots:
  void nextMonth();

  signals:
    void updated();
};

#endif // GAMECONTROLLER_H