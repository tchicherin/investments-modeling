#pragma once
#include "Investment.h"

class DepositInvestment : public Investment {
    double principal_;     // тело депозита
    double annualRate_;    // годовая ставка, 0.08 = 8%
    int monthsLeft_;       // месяцев до пересмотра/пролонгации
    int termMonths_;       // исходный срок для цикличной пролонгации
public:
    DepositInvestment(const QString& name, double amount, double annualRate, int months);

    QString type() const override { return "Deposit"; }
    double marketValue(const class Market& m) const override;
    double stepMonth(const class Market& m) override;
};
