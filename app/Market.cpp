#include "Market.h"
#include <algorithm>

void Market::randomizeMonth() {
    // Акции: небольшой дрейф + волатильность
    lastPrices_ = {};
    for (auto &c : companies_) {
        lastPrices_.push_back(c.price);
        double change = Rng::normal(0.005, 0.05); // ~0.5% средний рост, 5% волатильность
        c.price = std::max(1.0, c.price * (1.0 + change));
    }

    // Валюты: небольшие колебания ±2%
    for (auto &cur : currencies_) {
        double drift = Rng::uniform(-0.02, 0.02);
        cur.rate = std::max(0.01, cur.rate * (1.0 + drift));
    }

    // Облигации: небольшие колебания доходности ±0.5%
    for (auto &b : bonds_) {
        double drift = Rng::uniform(-0.005, 0.005);
        b.annualYield = std::clamp(b.annualYield + drift, 0.0, 1.0);
    }

    // Металлы: ±3%
    for (auto &m : metals_) {
        double drift = Rng::uniform(-0.03, 0.03);
        m.price = std::max(1.0, m.price * (1.0 + drift));
    }
}

// --- Companies
void Market::addCompany(const QString &name, double price) {
    companies_.push_back({name, price});
}
void Market::removeCompanyAt(int index) {
    if (index >= 0 && index < (int)companies_.size())
        companies_.erase(companies_.begin() + index);
}
void Market::setCompanyPrice(int index, double price) {
    if (index >= 0 && index < (int)companies_.size())
        companies_[index].price = price;
}

// --- Currencies
void Market::addCurrency(const QString &name, double rate) {
    currencies_.push_back({name, rate});
}
void Market::removeCurrencyAt(int index) {
    if (index >= 0 && index < (int)currencies_.size())
        currencies_.erase(currencies_.begin() + index);
}
void Market::setCurrencyRate(int index, double rate) {
    if (index >= 0 && index < (int)currencies_.size())
        currencies_[index].rate = rate;
}

// --- Bonds
void Market::addBond(const QString &name, double annualYield) {
    bonds_.push_back({name, annualYield});
}
void Market::removeBondAt(int index) {
    if (index >= 0 && index < (int)bonds_.size())
        bonds_.erase(bonds_.begin() + index);
}
void Market::setBondYield(int index, double annualYield) {
    if (index >= 0 && index < (int)bonds_.size())
        bonds_[index].annualYield = annualYield;
}

// --- Metals
void Market::addMetal(const QString &name, double price) {
    metals_.push_back({name, price});
}
void Market::removeMetalAt(int index) {
    if (index >= 0 && index < (int)metals_.size())
        metals_.erase(metals_.begin() + index);
}
void Market::setMetalPrice(int index, double price) {
    if (index >= 0 && index < (int)metals_.size())
        metals_[index].price = price;
}