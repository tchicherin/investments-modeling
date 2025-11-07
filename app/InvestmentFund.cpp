#include "InvestmentFund.h"
#include "Market.h"
#include "DepositInvestment.h"
#include "StockInvestment.h"
#include <algorithm>

#include "BondInvestment.h"
#include "CurrencyInvestment.h"
#include "MetalInvestment.h"

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

  // проверяем, есть ли такая компания в рынке
  bool found = false;
  for (const auto &c : m.companies()) {
    if (c.name == name) { found = true; break; }
  }
  if (!found) return false;

  cash_ -= amount;
  portfolio_.add(std::make_shared<StockInvestment>(name, name, amount, m));
  return true;
}

bool InvestmentFund::buyCurrency(const QString &name, double amount, double monthlyRate) {
  if (amount <= 0.0 || amount > cash_) return false;
  cash_ -= amount;
  portfolio_.add(std::make_shared<CurrencyInvestment>(name, amount, monthlyRate));
  return true;
}

bool InvestmentFund::buyBond(const QString &name, double amount, double annualYield) {
  if (amount <= 0.0 || amount > cash_) return false;
  cash_ -= amount;
  portfolio_.add(std::make_shared<BondInvestment>(name, amount, annualYield));
  return true;
}

bool InvestmentFund::buyMetal(const QString &name, double amount, const Market &m) {
  if (amount <= 0.0 || amount > cash_) return false;
  cash_ -= amount;

  // Найдем текущую цену металла
  double price = 60.0;
  for (const auto &mInfo : m.metals()) {
    if (mInfo.name == name) {
      price = mInfo.price;
      break;
    }
  }

  portfolio_.add(std::make_shared<MetalInvestment>(name, amount, price));
  return true;
}

void InvestmentFund::sellInvestment(int index, const Market &m) {
  if (index < 0 || index >= portfolio_.size()) return;
  const double val = portfolio_.items()[index]->marketValue(m);
  cash_ += val;
  portfolio_.removeAt(index);
}
