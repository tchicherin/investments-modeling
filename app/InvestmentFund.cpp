#include "InvestmentFund.h"
#include "Market.h"
#include "DepositInvestment.h"
#include "StockInvestment.h"
#include <algorithm>

#include "BondInvestment.h"
#include "CurrencyInvestment.h"
#include "MetalInvestment.h"

InvestmentFund::InvestmentFund(const double initialCapital) : cash_(initialCapital) {}

double InvestmentFund::equity(const Market &m) const {
  return cash_ + portfolio_.totalValue(m);
}

// Существующий простой метод (оставляем, он может вызывать detailed)
double InvestmentFund::simulateMonth(Market &market, double taxRate) {
  auto r = simulateMonthDetailed(market, taxRate, 0);
  return r.portfolioProfit;
}

MonthResult InvestmentFund::simulateMonthDetailed(Market &market, double taxRate, int monthIndex) {
  MonthResult res;
  res.monthIndex = monthIndex;
  res.equityBefore = equity(market);
  res.cashBefore = cash_;

  // Разбиваем по типам (если нужно)
  double profitDeposits = 0.0;
  double profitStocks = 0.0;
  double profitBonds = 0.0;
  double profitCurrency = 0.0;
  double profitMetals = 0.0;

  // Пройдёмся по портфелю и вызовем stepMonth для каждого инвестирования.
  // При желании здесь можно типизировать (через dynamic_cast) и суммировать по категориям.
  double portfolioProfit = 0.0;
  for (const auto &inv : portfolio_.items()) {
    // получаем прибыль/убыток от текущей позиции
    double p = inv->stepMonth(market);
    portfolioProfit += p;

    // попытка классификации по type() — не самая быстрая, но понятная
    QString t = inv->type();
    if (t == "Deposit") profitDeposits += p;
    else if (t == "Stock") profitStocks += p;
    else if (t == "Bond") profitBonds += p;
    else if (t == "Currency") profitCurrency += p;
    else if (t == "Metal") profitMetals += p;
  }

  res.portfolioProfit = portfolioProfit;

  // налог
  double tax = 0.0;
  if (portfolioProfit > 0.0) {
    tax = portfolioProfit * taxRate;
    cash_ -= tax;
  }
  res.taxPaid = tax;

  // Аккумулируем чистую прибыль (после налога) в totalProfit_
  totalProfit_ += (portfolioProfit - tax);

  // Модель притоков/оттоков: простой коэффициент зависимости от прибыли
  // (можно вынести в параметры моделирования)
  const double flowFactor = 0.5;
  double flow = flowFactor * portfolioProfit;
  cash_ += flow;
  res.flow = flow;

  // Обновляем cash и equity
  res.cashAfter = cash_;
  res.equityAfter = equity(market);
  res.netProfit = portfolioProfit - tax + flow;

  // Заполним детализацию
  res.profitDeposits = profitDeposits;
  res.profitStocks = profitStocks;
  res.profitBonds = profitBonds;
  res.profitCurrency = profitCurrency;
  res.profitMetals = profitMetals;

  return res;
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
