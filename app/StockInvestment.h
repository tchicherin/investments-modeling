#ifndef STOCK_INVESTMENT_H
#define STOCK_INVESTMENT_H

#include "Investment.h"
#include <QString>

class Market;

class StockInvestment : public Investment {
  QString assetName_; // имя акции в Market
  double shares_; // количество купленных акций
  double lastPrice_; // цена при последнем наблюдении

 public:
  // amount инвестируется по текущей цене assetName в Market — считаем количество shares
  StockInvestment(const QString &name, const QString &assetName, double amount, const Market &m);

  [[nodiscard]] QString type() const override { return "Stock"; }

  [[nodiscard]] double marketValue(const Market &m) const override;

  double stepMonth(const Market &m) override;
  [[nodiscard]] const QString &assetName() const { return assetName_; }
};

#endif // STOCK_INVESTMENT_H
