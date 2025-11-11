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

public:
    explicit MainWindow(GameController *controller, QWidget *parent = nullptr);

private:
    GameController *gc_;

    // Main tabs
    QTabWidget *tabs_;

    // --- Simulation tab widgets
    QLabel *lblMonth_;
    QLabel *lblCapital_;
    QLabel *lblCash_;
    QLabel *lblLastProfit_;
    QLabel *lblTaxRate_;
    QLabel *lblDepRate_;

    // Asset tabs inside Simulation
    QTabWidget *assetTabs_;
    QTableWidget *tblStockPrices_; // Stocks tab
    QTableWidget *tblBondPrices_;  // Bonds tab
    QTableWidget *tblMetalPrices_; // Metals tab

    QTableWidget *table_; // portfolio table

    QPushButton *btnNext_;
    QPushButton *btnAddDep_;
    QPushButton *btnAddStock_;
    QPushButton *btnAddCurrencySim_;
    QPushButton *btnAddBondSim_;
    QPushButton *btnAddMetalSim_;
    QPushButton *btnSell_;

    // --- Settings tab widgets
    QSpinBox *spinTotalMonths_;
    QDoubleSpinBox *spinInitialCapital_;
    QDoubleSpinBox *spinTaxRate_;
    QDoubleSpinBox *spinDepRate_;

    // Companies (settings)
    QTableWidget *tblCompanies_;
    QLineEdit *editCompanyName_;
    QDoubleSpinBox *spinCompanyPrice_;
    QPushButton *btnAddCompany_;
    QPushButton *btnRemoveCompany_;

    // Currencies (settings)
    QTableWidget *tblCurrencies_;
    QPushButton *btnAddCurrency_;
    QPushButton *btnRemoveCurrency_;

    // Bonds (settings)
    QTableWidget *tblBonds_;
    QPushButton *btnAddBond_;
    QPushButton *btnRemoveBond_;

    // Metals (settings)
    QTableWidget *tblMetals_;
    QPushButton *btnAddMetal_;
    QPushButton *btnRemoveMetal_;

    QPushButton *btnApplySettings_;

    bool summaryShown_ = false;

    // Randomization controls (optional)
    QDoubleSpinBox *spinStockDriftMean_;
    QDoubleSpinBox *spinStockDriftStd_;
    QDoubleSpinBox *spinCurrencyRange_;
    QDoubleSpinBox *spinMetalRange_;

    // helpers
    void setupUi();
    void setupSimulationTab(QWidget *w);
    void setupSettingsTab(QWidget *w);

    void refresh();
    void refreshCompaniesTable();
    void refreshAssetTables();

    // dialogs
    void addDepositDialog();
    void addStockDialog();

private slots:
    // settings slots
    void onAddCompany();
    void onRemoveCompany();
    void onAddCurrency();
    void onRemoveCurrency();
    void onAddBond();
    void onRemoveBond();
    void onAddMetal();
    void onRemoveMetal();
    void onApplySettings();

    // simulation quick-buy slots
    void onAddCurrencySim();
    void onAddBondSim();
    void onAddMetalSim();
};

#endif // MAIN_WINDOW_H