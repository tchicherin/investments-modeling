#pragma once
#include <vector>
#include <memory>

class Investment;
class Market;

class Portfolio {
    std::vector<std::shared_ptr<Investment>> items_;
public:
    const std::vector<std::shared_ptr<Investment>>& items() const { return items_; }
    void add(const std::shared_ptr<Investment>& inv);
    void removeAt(int index);
    double totalValue(const Market& m) const;
    double stepAll(const Market& m); // суммарная прибыль за месяц
    int size() const { return static_cast<int>(items_.size()); }
};
