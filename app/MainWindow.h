#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include "GameController.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
  GameController *gc_;

  QLabel *lblMonth_;
  QLabel *lblCapital_;
  QLabel *lblCash_;
  QLabel *lblLastProfit_;
  QLabel *lblTaxRate_;
  QLabel *lblStockPrice_;
  QLabel *lblDepRate_;

  QTableWidget *table_;

  QPushButton *btnNext_;
  QPushButton *btnAddDep_;
  QPushButton *btnAddStock_;
  QPushButton *btnSell_;

  void setupUi();

  void refresh();

  void addDepositDialog();

  void addStockDialog();

public:
  explicit MainWindow(GameController *controller, QWidget *parent = nullptr);
};

#endif // MAIN_WINDOW_H
