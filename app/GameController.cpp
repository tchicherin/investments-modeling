#include "GameController.h"
#include "Utils.h"

#include "GameController.h"
#include "Market.h"
#include "Utils.h"
#include <numeric>
#include <algorithm>
#include <cmath>

GameController::GameController(double initialCapital, int totalMonths, double taxRate, unsigned int seed, QObject *parent)
  : QObject(parent),
    fund_(initialCapital),
    totalMonths_(totalMonths),
    taxRate_(taxRate) {
  // Если хочешь использовать seed для Rng, можно модифицировать Utils::gen() — но
  // для простоты оставляем стандартную инициализацию.
}

void GameController::nextMonth() {
  if (month_ >= totalMonths_) {
    emit updated();
    return;
  }

  // Применяем параметры drift/volatility к рынку перед шагом
  // Для этого сделаем небольшный интерфейс: Market::randomizeMonth(...) — если не хочется менять Market,
  // можно временно изменить глобальные Rng параметры; здесь оставим вызов старого randomizeMonth(),
  // но если хочешь — можно расширить Market.randomizeMonth(mean,stddev,...)

  market_.randomizeMonth();

  // Симуляция месяца с детальными данными
  MonthResult r = fund_.simulateMonthDetailed(market_, taxRate_, month_);
  lastMonthProfit_ = r.portfolioProfit;
  history_.push_back(r);

  month_ += 1;
  emit updated();
}

GameController::SummaryStats GameController::computeSummary() const {
  SummaryStats s;
  if (history_.empty()) return s;

  s.initialEquity = history_.front().equityBefore;
  s.finalEquity = history_.back().equityAfter;
  s.totalReturn = (s.finalEquity - s.initialEquity) / std::max(1.0, s.initialEquity);

  // average monthly return (arithmetic) and volatility
  std::vector<double> monthlyReturns;
  monthlyReturns.reserve(history_.size());
  for (const auto &m : history_) {
    double before = m.equityBefore;
    double after = m.equityAfter;
    if (before <= 0.0) continue;
    monthlyReturns.push_back((after - before) / before);
  }
  if (!monthlyReturns.empty()) {
    double sum = std::accumulate(monthlyReturns.begin(), monthlyReturns.end(), 0.0);
    double mean = sum / monthlyReturns.size();
    s.avgMonthlyReturn = mean;
    double var = 0.0;
    for (double x : monthlyReturns) var += (x - mean) * (x - mean);
    var /= monthlyReturns.size();
    s.volatilityMonthly = std::sqrt(var);

    // CAGR approx
    double months = static_cast<double>(monthlyReturns.size());
    if (months > 0.0 && s.initialEquity > 0.0) {
      s.annualizedReturn = std::pow(1.0 + s.totalReturn, 12.0 / months) - 1.0;
    }

    // % profitable months
    int cnt = 0;
    for (double x : monthlyReturns) if (x > 0.0) ++cnt;
    s.percentProfitableMonths = 100.0 * static_cast<double>(cnt) / monthlyReturns.size();
  }

  // total inflow & tax
  double inflow = 0.0, tax = 0.0;
  double peak = s.initialEquity;
  double maxDD = 0.0;
  double runningPeak = s.initialEquity;
  for (const auto &m : history_) {
    inflow += m.flow;
    tax += m.taxPaid;
    double val = m.equityAfter;
    if (val > runningPeak) runningPeak = val;
    double dd = (runningPeak - val) / runningPeak;
    if (dd > maxDD) maxDD = dd;
  }
  s.totalInflow = inflow;
  s.totalTaxPaid = tax;
  s.maxDrawdown = maxDD;

  return s;
}
