#include "Market.h"
#include "Utils.h"
#include <algorithm>

void Market::randomizeMonth() {
  // Акции: средний дрейф 0.5%/мес, волатильность ~5%/мес
  double stockMonthlyChange = Rng::normal(0.005, 0.05);
  cur_.stockPrice = std::max(1.0, cur_.stockPrice * (1.0 + stockMonthlyChange));

  // Депозитная ставка: медленный дрейф ±0.5 п.п. годовых в месяц
  double depDrift = Rng::uniform(-0.005, 0.005);
  cur_.depositRate = std::clamp(cur_.depositRate + depDrift, 0.01, 0.20);
}
