#include "GameController.h"

GameController::GameController(double initialCapital, int totalMonths, double taxRate, QObject *parent)
  : QObject(parent),
    fund_(initialCapital),
    totalMonths_(totalMonths),
    taxRate_(taxRate) {
}

void GameController::nextMonth() {
  if (month_ >= totalMonths_) {
    emit updated();
    return;
  }
  market_.randomizeMonth();
  lastMonthProfit_ = fund_.simulateMonth(market_, taxRate_);
  month_ += 1;
  emit updated();
}
