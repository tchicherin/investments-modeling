#pragma once
#include "Investment.h"

class StockInvestment : public Investment {
    double shares_;        // количество купленных «акций» индекса
    double lastPrice_;     // последняя цена для расчёта Δ
public:
    // amount инвестируется по текущей цене — считаем количество shares
    StockInvestment(const QString& name, double amount, const class Market& m);

    QString type() const override { return "Stock"; }
    double marketValue(const class Market& m) const override;
    double stepMonth(const class Market& m) override;
};
