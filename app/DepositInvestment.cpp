#include "DepositInvestment.h"
#include "Market.h"
#include <algorithm>

DepositInvestment::DepositInvestment(const QString& name, double amount,
                                     double annualRate, int months)
    : Investment(name),
      principal_(amount),
      annualRate_(annualRate),
      monthsLeft_(months),
      termMonths_(months) {}

double DepositInvestment::marketValue(const Market&) const {
    return principal_;
}

double DepositInvestment::stepMonth(const Market& mkt) {
    // Начисляем месячные проценты
    double monthlyRate = annualRate_ / 12.0;
    double interest = principal_ * monthlyRate;
    principal_ += interest;

    // Пересмотр ставки по окончании срока
    monthsLeft_ -= 1;
    if (monthsLeft_ <= 0) {
        annualRate_ = std::clamp(mkt.current().depositRate, 0.01, 0.25);
        monthsLeft_ = termMonths_;
    }
    return interest; // прибыль за месяц
}
