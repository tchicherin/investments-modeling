#ifndef MARKET_H
#define MARKET_H

#include <QString>
#include <vector>

struct MarketSnapshot {
  double stockPrice = 100.0; // базовая цена «индекса»
  double depositRate = 0.08; // годовая ставка депозитов (8%)
  double metalPrice = 60.0; // задел на будущее
  double bondYield = 0.06; // задел на будущее
};

struct StockInfo {
  QString name;
  double price = 100.0;
};

class Market {
  MarketSnapshot cur_;
  std::vector<StockInfo> companies_; // пользовательские компании

public:
  Market();

  [[nodiscard]] const MarketSnapshot &current() const { return cur_; }

  void randomizeMonth(); // обновляет параметры на один месяц

  // Companies API
  const std::vector<StockInfo> &companies() const { return companies_; }
  std::vector<StockInfo> &companies() { return companies_; }

  void addCompany(const QString &name, double price);
  void removeCompanyAt(int index);
  void setCompanyPrice(int index, double price);

  // setters/getters convenience
  void setDepositRate(double r) { cur_.depositRate = r; }
  double depositRate() const { return cur_.depositRate; }

  // Keep compatibility: returns aggregate stockPrice (if companies present, average; else cur_.stockPrice)
  double aggregateStockPrice() const;
};

#endif // MARKET_H