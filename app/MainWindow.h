#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

#include "GameController.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
  GameController *gc_;

  // Tab widget
  QTabWidget *tabs_;

  // Main simulation tab widgets
  QLabel *lblMonth_;
  QLabel *lblCapital_;
  QLabel *lblCash_;
  QLabel *lblLastProfit_;
  QLabel *lblTaxRate_;
  QTableWidget *tblStockPrices_;
  QLabel *lblDepRate_;

  QTableWidget *table_;

  QPushButton *btnNext_;
  QPushButton *btnAddDep_;
  QPushButton *btnAddStock_;
  QPushButton *btnSell_;

  // Settings tab widgets
  QSpinBox *spinTotalMonths_;
  QDoubleSpinBox *spinInitialCapital_;
  QDoubleSpinBox *spinTaxRate_;
  QDoubleSpinBox *spinDepRate_;

  QTableWidget *tblCompanies_;
  QLineEdit *editCompanyName_;
  QDoubleSpinBox *spinCompanyPrice_;
  QPushButton *btnAddCompany_;
  QPushButton *btnRemoveCompany_;
  QPushButton *btnApplySettings_;

  void setupUi();

  void setupSimulationTab(QWidget *w);
  void setupSettingsTab(QWidget *w);

  void refresh();

  void refreshCompaniesTable();

  void addDepositDialog();

  void addStockDialog();

private slots:
  void onAddCompany();
  void onRemoveCompany();
  void onApplySettings();

public:
  explicit MainWindow(GameController *controller, QWidget *parent = nullptr);
};

#endif // MAIN_WINDOW_H
