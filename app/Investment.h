#pragma once
#include <QString>

class Market;

// Базовый класс инвестиции
class Investment {
protected:
    QString name_;
public:
    explicit Investment(QString name) : name_(std::move(name)) {}
    virtual ~Investment() = default;

    const QString& name() const { return name_; }
    virtual QString type() const = 0;

    // Текущая рыночная стоимость инвестиции
    virtual double marketValue(const Market& m) const = 0;

    // Месячный шаг симуляции: вернуть прибыль (Δстоимости, проценты и т.п.)
    virtual double stepMonth(const Market& m) = 0;
};
