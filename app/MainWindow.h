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

    // Tabs
    QTabWidget *tabs_;

    // --- Simulation tab widgets
    QLabel *lblMonth_;
    QLabel *lblCapital_;
    QLabel *lblCash_;
    QLabel *lblLastProfit_;
    QLabel *lblTaxRate_;
    QLabel *lblDepRate_;

    QTableWidget *tblStockPrices_;
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

    // Companies
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

    // UI setup / refresh helpers
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
    // companies
    void onAddCompany();
    void onRemoveCompany();

    // currencies
    void onAddCurrency();        // settings Add
    void onRemoveCurrency();     // settings Remove
    void onAddCurrencySim();     // simulation quick-buy

    // bonds
    void onAddBond();            // settings Add
    void onRemoveBond();         // settings Remove
    void onAddBondSim();         // simulation quick-buy

    // metals
    void onAddMetal();           // settings Add
    void onRemoveMetal();        // settings Remove
    void onAddMetalSim();        // simulation quick-buy

    // apply settings
    void onApplySettings();
};

#endif // MAIN_WINDOW_H
