#ifndef INVESTMENT_H
#define INVESTMENT_H

#include <QString>

class Market;

// Базовый класс инвестиции
class Investment {
 protected:
  QString name_;

 public:
  explicit Investment(QString name) : name_(std::move(name)) {
  }

  virtual ~Investment() = default;

  [[nodiscard]] const QString &name() const { return name_; }

  [[nodiscard]] virtual QString type() const = 0;

  // Текущая рыночная стоимость инвестиции
  [[nodiscard]] virtual double marketValue(const Market &m) const = 0;

  // Месячный шаг симуляции: вернуть прибыль (Δстоимости, проценты и т.п.)
  virtual double stepMonth(const Market &m) = 0;
};

#endif // INVESTMENT_H
