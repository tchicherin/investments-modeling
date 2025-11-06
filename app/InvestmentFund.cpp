#include "InvestmentFund.h"
#include "Market.h"
#include "DepositInvestment.h"
#include "StockInvestment.h"
#include <algorithm>

InvestmentFund::InvestmentFund(const double initialCapital) : cash_(initialCapital) {
}

double InvestmentFund::equity(const Market &m) const {
  return cash_ + portfolio_.totalValue(m);
}

double InvestmentFund::simulateMonth(Market &market, double taxRate) {
  // Прибыль портфеля за месяц
  double portfolioProfit = portfolio_.stepAll(market);

  // Налог на прибыль
  double tax = 0.0;
  if (portfolioProfit > 0.0) {
    tax = portfolioProfit * taxRate;
    cash_ -= tax;
    totalProfit_ += portfolioProfit - tax;
  } else {
    totalProfit_ += portfolioProfit;
  }

  // Притоки/оттоки: 50% от прибыли/убытка
  double k = 0.5;
  double flow = k * portfolioProfit;
  cash_ += flow;

  return portfolioProfit; // до налога
}

bool InvestmentFund::buyDeposit(const QString &name, double amount, double annualRate, int months) {
  if (amount <= 0.0 || amount > cash_) return false;
  cash_ -= amount;
  portfolio_.add(std::make_shared<DepositInvestment>(name, amount, annualRate, months));
  return true;
}

bool InvestmentFund::buyStock(const QString &name, double amount, const Market &m) {
  if (amount <= 0.0 || amount > cash_) return false;
  cash_ -= amount;
  portfolio_.add(std::make_shared<StockInvestment>(name, amount, m));
  return true;
}

void InvestmentFund::sellInvestment(int index, const Market &m) {
  if (index < 0 || index >= portfolio_.size()) return;
  const double val = portfolio_.items()[index]->marketValue(m);
  cash_ += val;
  portfolio_.removeAt(index);
}
