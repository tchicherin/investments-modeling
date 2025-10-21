#include "Portfolio.h"
#include "Investment.h"
#include "Market.h"

void Portfolio::add(const std::shared_ptr<Investment>& inv) {
    items_.push_back(inv);
}

void Portfolio::removeAt(int index) {
    if (index < 0 || index >= (int)items_.size()) return;
    items_.erase(items_.begin() + index);
}

double Portfolio::totalValue(const Market& m) const {
    double sum = 0.0;
    for (const auto& it : items_) sum += it->marketValue(m);
    return sum;
}

double Portfolio::stepAll(const Market& m) {
    double profit = 0.0;
    for (auto& it : items_) profit += it->stepMonth(m);
    return profit;
}
