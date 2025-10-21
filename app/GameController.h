#pragma once
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
    explicit GameController(double initialCapital, int totalMonths, double taxRate, QObject* parent = nullptr);

    const Market& market() const { return market_; }
    Market& market() { return market_; }

    const InvestmentFund& fund() const { return fund_; }
    InvestmentFund& fund() { return fund_; }

    int month() const { return month_; }
    int totalMonths() const { return totalMonths_; }
    double lastMonthProfit() const { return lastMonthProfit_; }
    double taxRate() const { return taxRate_; }

public slots:
    void nextMonth();

signals:
    void updated();
};
