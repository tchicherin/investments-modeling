#ifndef MARKET_H
#define MARKET_H

#include <QString>
#include <vector>
#include "Utils.h"

struct MarketSnapshot {
    double depositRate = 0.08;   // годовая ставка депозитов
};

struct StockInfo {
    QString name;
    double price = 100.0;
};

struct CurrencyInfo {
    QString name;
    double rate = 1.0; // курс относительно базовой валюты
};

struct BondInfo {
    QString name;
    double annualYield = 0.05; // годовая доходность
};

struct MetalInfo {
    QString name;
    double price = 50.0;
};

class Market {
    MarketSnapshot cur_;
    std::vector<StockInfo> companies_;
    std::vector<double> lastPrices_;
    std::vector<CurrencyInfo> currencies_;
    std::vector<BondInfo> bonds_;
    std::vector<MetalInfo> metals_;

public:
    Market() = default;

    const MarketSnapshot &current() const { return cur_; }

    void randomizeMonth(); // обновление месяца (акции, курс валют, доходность)

    // Companies
    const std::vector<StockInfo> &companies() const { return companies_; }
    std::vector<StockInfo> &companies() { return companies_; }
    const std::vector<double> &lastPrices() const { return lastPrices_; }

    void addCompany(const QString &name, double price);
    void removeCompanyAt(int index);
    void setCompanyPrice(int index, double price);

    // Currencies
    const std::vector<CurrencyInfo> &currencies() const { return currencies_; }
    void addCurrency(const QString &name, double rate);
    void removeCurrencyAt(int index);
    void setCurrencyRate(int index, double rate);

    // Bonds
    const std::vector<BondInfo> &bonds() const { return bonds_; }
    void addBond(const QString &name, double annualYield);
    void removeBondAt(int index);
    void setBondYield(int index, double annualYield);

    // Metals
    const std::vector<MetalInfo> &metals() const { return metals_; }
    void addMetal(const QString &name, double price);
    void removeMetalAt(int index);
    void setMetalPrice(int index, double price);

    // Deposit setter
    void setDepositRate(double r) { cur_.depositRate = r; }
};
#endif // MARKET_H
