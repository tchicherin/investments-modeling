#include "Market.h"
#include "Utils.h"
#include <algorithm>

void Market::randomizeMonth() {
    // Акции: логнормальная аппроксимация через нормальные изменения %
    // Средний дрейф 0.5% в месяц, вола ~5% в месяц
    double stockMonthlyChange = Rng::normal(0.005, 0.05);
    cur_.stockPrice = std::max(1.0, cur_.stockPrice * (1.0 + stockMonthlyChange));

    // Ставки депозитов: медленный дрейф +/-0.5% годовых в месяц
    double depDrift = Rng::uniform(-0.005, 0.005);
    cur_.depositRate = std::clamp(cur_.depositRate + depDrift, 0.01, 0.20);
}
