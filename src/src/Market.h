#pragma once
#include <QString>

struct MarketSnapshot {
    double stockPrice = 100.0;   // базовая цена «индекса»
    double depositRate = 0.08;   // годовая ставка депозитов (8%)
    double metalPrice = 60.0;    // не используется в MVP, задел на будущее
    double bondYield = 0.06;     // не используется в MVP, задел на будущее
};

class Market {
    MarketSnapshot cur_;
public:
    const MarketSnapshot& current() const { return cur_; }

    // Случайное месячное обновление рынка:
    // акции: стох. волатильность; депозитная ставка: плавные дрейфы
    void randomizeMonth();
};
