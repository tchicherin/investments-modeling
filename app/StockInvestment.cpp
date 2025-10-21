#include "StockInvestment.h"
#include "Market.h"
#include <algorithm>

StockInvestment::StockInvestment(const QString& name, double amount, const Market& m)
    : Investment(name) {
    double price = std::max(1.0, m.current().stockPrice);
    shares_ = amount / price;
    lastPrice_ = price;
}

double StockInvestment::marketValue(const Market& m) const {
    return shares_ * std::max(1.0, m.current().stockPrice);
}

double StockInvestment::stepMonth(const Market& m) {
    double newPrice = std::max(1.0, m.current().stockPrice);
    double oldValue = shares_ * lastPrice_;
    double newValue = shares_ * newPrice;
    lastPrice_ = newPrice;
    return newValue - oldValue; // прибыль/убыток за месяц
}
