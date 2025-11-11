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

  // история
  std::vector<MonthResult> history_;

  // параметры генерации случайностей (диапазоны) — задаются из UI
  double stockMonthlyDriftMean_ = 0.005;
  double stockMonthlyDriftStd_ = 0.05;
  double currencyDriftRange_ = 0.02;
  double metalDriftRange_ = 0.03;

 public:
  explicit GameController(double initialCapital, int totalMonths, double taxRate, unsigned int seed, QObject *parent = nullptr);

  // Доступ к истории
  [[nodiscard]] const std::vector<MonthResult> &history() const { return history_; }

  // Итоговая статистика (вспомогательный)
  struct SummaryStats {
    double finalEquity = 0.0;
    double initialEquity = 0.0;
    double totalReturn = 0.0;
    double annualizedReturn = 0.0; // CAGR approximate
    double avgMonthlyReturn = 0.0;
    double volatilityMonthly = 0.0;
    double percentProfitableMonths = 0.0;
    double maxDrawdown = 0.0;
    double totalInflow = 0.0;
    double totalTaxPaid = 0.0;
  };

  [[nodiscard]] SummaryStats computeSummary() const;

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

  // setters for randomization params
  void setStockDriftParams(double mean, double stddev) { stockMonthlyDriftMean_ = mean; stockMonthlyDriftStd_ = stddev; }
  void setCurrencyDriftRange(double r) { currencyDriftRange_ = r; }
  void setMetalDriftRange(double r) { metalDriftRange_ = r; }

 public slots:
  void nextMonth();

  signals:
    void updated();
};

#endif // GAMECONTROLLER_H