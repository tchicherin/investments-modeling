#include "Market.h"
#include "Utils.h"
#include <algorithm>

Market::Market() {
  // оставляем cur_.stockPrice и depositRate по умолчанию, companies_ пусты
}

void Market::addCompany(const QString &name, double price) {
  StockInfo s;
  s.name = name;
  s.price = price > 0.0 ? price : 1.0;
  companies_.push_back(s);

  // обновим агрегатную цену
  cur_.stockPrice = aggregateStockPrice();
}

void Market::removeCompanyAt(int index) {
  if (index < 0 || index >= static_cast<int>(companies_.size())) return;
  companies_.erase(companies_.begin() + index);
  cur_.stockPrice = aggregateStockPrice();
}

void Market::setCompanyPrice(int index, double price) {
  if (index < 0 || index >= static_cast<int>(companies_.size())) return;
  companies_[index].price = price > 0.0 ? price : 0.01;
  cur_.stockPrice = aggregateStockPrice();
}

double Market::aggregateStockPrice() const {
  if (companies_.empty()) return cur_.stockPrice;
  double sum = 0.0;
  for (const auto &c : companies_) sum += c.price;
  return sum / static_cast<double>(companies_.size());
}

void Market::randomizeMonth() {
  lastPrices_.clear();
  for (const auto &c : companies_) lastPrices_.push_back(c.price);
  // Если есть компании — обновляем их цены случайным блужданием.
  if (!companies_.empty()) {
    for (auto &c : companies_) {
      // нормальное приращение: средний дрейф 0.5% мес, волатильность 5% мес
      double change = Rng::normal(0.005, 0.05);
      double factor = 1.0 + change;
      if (factor <= 0.0) factor = 0.01;
      c.price = std::max(0.01, c.price * factor);
    }
    // Обновляем агрегатную stockPrice
    cur_.stockPrice = aggregateStockPrice();
  } else {
    // Старая логика: если компаний нет — используем cur_.stockPrice (как раньше)
    double stockMonthlyChange = Rng::normal(0.005, 0.05);
    cur_.stockPrice = std::max(1.0, cur_.stockPrice * (1.0 + stockMonthlyChange));
  }

  // Депозитная ставка: медленный дрейф ±0.5 п.п. годовых в месяц
  double depDrift = Rng::uniform(-0.005, 0.005);
  cur_.depositRate = std::clamp(cur_.depositRate + depDrift, 0.01, 0.20);
}