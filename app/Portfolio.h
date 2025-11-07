#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <memory>

class Investment;
class Market;

class Portfolio {
  std::vector<std::shared_ptr<Investment>> items_;

public:
  [[nodiscard]] const std::vector<std::shared_ptr<Investment>> &items() const { return items_; }

  void add(const std::shared_ptr<Investment> &inv);

  void removeAt(int index);

  [[nodiscard]] double totalValue(const Market &m) const;

  // stepAll изменяет состояние вложений (например lastPrice_), поэтому не const
  [[nodiscard]] double stepAll(const Market &m) const;
  [[nodiscard]] int size() const { return static_cast<int>(items_.size()); }
};

#endif // PORTFOLIO_H
