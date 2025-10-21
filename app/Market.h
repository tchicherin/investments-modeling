#pragma once

struct MarketSnapshot {
    double stockPrice = 100.0;   // базовая цена «индекса»
    double depositRate = 0.08;   // годовая ставка депозитов (8%)
    double metalPrice = 60.0;    // задел на будущее
    double bondYield  = 0.06;    // задел на будущее
};

class Market {
    MarketSnapshot cur_;
public:
    const MarketSnapshot& current() const { return cur_; }
    void randomizeMonth(); // обновляет параметры на один месяц
};
