#include "MainWindow.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QBrush>

#include "Investment.h"

MainWindow::MainWindow(GameController *controller, QWidget *parent)
    : QMainWindow(parent), gc_(controller) {
    setupUi();
    refresh();
    connect(gc_, &GameController::updated, this, &MainWindow::refresh);
}

void MainWindow::setupUi() {
    auto *central = new QWidget(this);
    setCentralWidget(central);
    setWindowTitle("Investment Portfolio Simulator");

    tabs_ = new QTabWidget(this);

    auto *simTab = new QWidget(this);
    setupSimulationTab(simTab);
    tabs_->addTab(simTab, "Simulation");

    auto *settingsTab = new QWidget(this);
    setupSettingsTab(settingsTab);
    tabs_->addTab(settingsTab, "Settings");

    auto *layout = new QVBoxLayout();
    layout->addWidget(tabs_);
    central->setLayout(layout);
}

/* ---------------- Simulation tab ---------------- */
void MainWindow::setupSimulationTab(QWidget *w) {
    // labels
    lblMonth_ = new QLabel(this);
    lblCapital_ = new QLabel(this);
    lblCash_ = new QLabel(this);
    lblLastProfit_ = new QLabel(this);
    lblTaxRate_ = new QLabel(this);
    lblDepRate_ = new QLabel(this);

    QFormLayout *topInfo = new QFormLayout();
    topInfo->addRow("Month:", lblMonth_);
    topInfo->addRow("Tax rate:", lblTaxRate_);
    topInfo->addRow("Deposit rate:", lblDepRate_);
    topInfo->addRow("Total equity:", lblCapital_);
    topInfo->addRow("Cash:", lblCash_);
    topInfo->addRow("Last month profit:", lblLastProfit_);

    // portfolio table
    table_ = new QTableWidget(this);
    table_->setColumnCount(4);
    table_->setHorizontalHeaderLabels(QStringList{"Type", "Name", "Value", "Notes"});
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // stock prices table
    tblStockPrices_ = new QTableWidget(this);
    tblStockPrices_->setColumnCount(3);
    tblStockPrices_->setHorizontalHeaderLabels(QStringList{"Company", "Price", "Δ"});
    tblStockPrices_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblStockPrices_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblStockPrices_->setSelectionMode(QAbstractItemView::NoSelection);

    // buttons
    btnNext_ = new QPushButton("▶ Next Month", this);
    btnAddDep_ = new QPushButton("➕ Add Deposit", this);
    btnAddStock_ = new QPushButton("➕ Add Stock", this);
    btnAddCurrencySim_ = new QPushButton("➕ Add Currency", this);
    btnAddBondSim_ = new QPushButton("➕ Add Bond", this);
    btnAddMetalSim_ = new QPushButton("➕ Add Metal", this);
    btnSell_ = new QPushButton("✖ Sell Selected", this);

    QHBoxLayout *btns = new QHBoxLayout();
    btns->addWidget(btnNext_);
    btns->addStretch();
    btns->addWidget(btnAddDep_);
    btns->addWidget(btnAddStock_);
    btns->addWidget(btnAddCurrencySim_);
    btns->addWidget(btnAddBondSim_);
    btns->addWidget(btnAddMetalSim_);
    btns->addWidget(btnSell_);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topInfo);
    mainLayout->addWidget(tblStockPrices_);
    mainLayout->addWidget(table_);
    mainLayout->addLayout(btns);
    w->setLayout(mainLayout);

    // connections
    connect(btnNext_, &QPushButton::clicked, gc_, &GameController::nextMonth);
    connect(btnAddDep_, &QPushButton::clicked, this, &MainWindow::addDepositDialog);
    connect(btnAddStock_, &QPushButton::clicked, this, &MainWindow::addStockDialog);
    connect(btnSell_, &QPushButton::clicked, [this]() {
        int sel = table_->currentRow();
        if (sel < 0) return;
        gc_->fund().sellInvestment(sel, gc_->market());
        refresh();
    });

    connect(btnAddCurrencySim_, &QPushButton::clicked, this, &MainWindow::onAddCurrencySim);
    connect(btnAddBondSim_, &QPushButton::clicked, this, &MainWindow::onAddBondSim);
    connect(btnAddMetalSim_, &QPushButton::clicked, this, &MainWindow::onAddMetalSim);
}

/* ---------------- Settings tab ---------------- */
void MainWindow::setupSettingsTab(QWidget *w) {
    // left: simulation params form
    spinTotalMonths_ = new QSpinBox(this);
    spinTotalMonths_->setRange(1, 120);
    spinTotalMonths_->setValue(gc_->totalMonths());

    spinInitialCapital_ = new QDoubleSpinBox(this);
    spinInitialCapital_->setRange(0.0, 1e12);
    spinInitialCapital_->setDecimals(2);
    spinInitialCapital_->setValue(gc_->fund().cash());

    spinTaxRate_ = new QDoubleSpinBox(this);
    spinTaxRate_->setRange(0.0, 1.0);
    spinTaxRate_->setDecimals(4);
    spinTaxRate_->setValue(gc_->taxRate());

    spinDepRate_ = new QDoubleSpinBox(this);
    spinDepRate_->setRange(0.0, 1.0);
    spinDepRate_->setDecimals(4);
    spinDepRate_->setValue(gc_->market().current().depositRate);

    QFormLayout *form = new QFormLayout();
    form->addRow("Total months:", spinTotalMonths_);
    form->addRow("Initial capital:", spinInitialCapital_);
    form->addRow("Tax rate:", spinTaxRate_);
    form->addRow("Deposit rate:", spinDepRate_);

    // --- Right side: companies + currencies + bonds + metals + apply
    // Companies
    tblCompanies_ = new QTableWidget(this);
    tblCompanies_->setColumnCount(2);
    tblCompanies_->setHorizontalHeaderLabels(QStringList{"Company", "Price"});
    tblCompanies_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    editCompanyName_ = new QLineEdit(this);
    spinCompanyPrice_ = new QDoubleSpinBox(this);
    spinCompanyPrice_->setRange(0.01, 1e9);
    spinCompanyPrice_->setDecimals(2);
    spinCompanyPrice_->setValue(100.0);

    btnAddCompany_ = new QPushButton("Add Company", this);
    btnRemoveCompany_ = new QPushButton("Remove Selected", this);

    QHBoxLayout *companyControls = new QHBoxLayout();
    companyControls->addWidget(editCompanyName_);
    companyControls->addWidget(spinCompanyPrice_);
    companyControls->addWidget(btnAddCompany_);
    companyControls->addWidget(btnRemoveCompany_);

    // Currencies
    tblCurrencies_ = new QTableWidget(this);
    tblCurrencies_->setColumnCount(2);
    tblCurrencies_->setHorizontalHeaderLabels(QStringList{"Currency", "Rate"});
    tblCurrencies_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblCurrencies_->setEditTriggers(QAbstractItemView::DoubleClicked);

    btnAddCurrency_ = new QPushButton("Add Currency", this);
    btnRemoveCurrency_ = new QPushButton("Remove Selected", this);
    QHBoxLayout *currencyControls = new QHBoxLayout();
    currencyControls->addWidget(btnAddCurrency_);
    currencyControls->addWidget(btnRemoveCurrency_);

    // Bonds
    tblBonds_ = new QTableWidget(this);
    tblBonds_->setColumnCount(2);
    tblBonds_->setHorizontalHeaderLabels(QStringList{"Bond", "Annual Yield"});
    tblBonds_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblBonds_->setEditTriggers(QAbstractItemView::DoubleClicked);

    btnAddBond_ = new QPushButton("Add Bond", this);
    btnRemoveBond_ = new QPushButton("Remove Selected", this);
    QHBoxLayout *bondControls = new QHBoxLayout();
    bondControls->addWidget(btnAddBond_);
    bondControls->addWidget(btnRemoveBond_);

    // Metals
    tblMetals_ = new QTableWidget(this);
    tblMetals_->setColumnCount(2);
    tblMetals_->setHorizontalHeaderLabels(QStringList{"Metal", "Price"});
    tblMetals_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblMetals_->setEditTriggers(QAbstractItemView::DoubleClicked);

    btnAddMetal_ = new QPushButton("Add Metal", this);
    btnRemoveMetal_ = new QPushButton("Remove Selected", this);
    QHBoxLayout *metalControls = new QHBoxLayout();
    metalControls->addWidget(btnAddMetal_);
    metalControls->addWidget(btnRemoveMetal_);

    btnApplySettings_ = new QPushButton("Apply Settings", this);

    // assemble right layout
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(tblCompanies_);
    rightLayout->addLayout(companyControls);
    rightLayout->addWidget(tblCurrencies_);
    rightLayout->addLayout(currencyControls);
    rightLayout->addWidget(tblBonds_);
    rightLayout->addLayout(bondControls);
    rightLayout->addWidget(tblMetals_);
    rightLayout->addLayout(metalControls);
    rightLayout->addStretch();
    rightLayout->addWidget(btnApplySettings_);

    // combine left + right
    QHBoxLayout *h = new QHBoxLayout();
    auto *leftWidget = new QWidget(this);
    leftWidget->setLayout(form);
    h->addWidget(leftWidget, 0);
    auto *rightWidget = new QWidget(this);
    rightWidget->setLayout(rightLayout);
    h->addWidget(rightWidget, 1);

    w->setLayout(h);

    // connections - settings
    connect(btnAddCompany_, &QPushButton::clicked, this, &MainWindow::onAddCompany);
    connect(btnRemoveCompany_, &QPushButton::clicked, this, &MainWindow::onRemoveCompany);

    connect(btnAddCurrency_, &QPushButton::clicked, this, &MainWindow::onAddCurrency);
    connect(btnRemoveCurrency_, &QPushButton::clicked, this, &MainWindow::onRemoveCurrency);

    connect(btnAddBond_, &QPushButton::clicked, this, &MainWindow::onAddBond);
    connect(btnRemoveBond_, &QPushButton::clicked, this, &MainWindow::onRemoveBond);

    connect(btnAddMetal_, &QPushButton::clicked, this, &MainWindow::onAddMetal);
    connect(btnRemoveMetal_, &QPushButton::clicked, this, &MainWindow::onRemoveMetal);

    connect(btnApplySettings_, &QPushButton::clicked, this, &MainWindow::onApplySettings);
}

/* ---------------- Dialogs ---------------- */
void MainWindow::addDepositDialog() {
    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Add Deposit", "Amount:", 10000.0, 0.0, 1e12, 2, &ok);
    if (!ok) return;
    double rate = QInputDialog::getDouble(this, "Add Deposit", "Annual rate (0.08):", 0.08, 0.001, 1.0, 3, &ok);
    if (!ok) return;
    int months = QInputDialog::getInt(this, "Add Deposit", "Term (months):", 12, 1, 120, 1, &ok);
    if (!ok) return;
    QString name = QInputDialog::getText(this, "Add Deposit", "Name:", QLineEdit::Normal, "Bank Deposit", &ok);
    if (!ok) return;

    if (!gc_->fund().buyDeposit(name, amount, rate, months)) {
        QMessageBox::warning(this, "Not enough cash", "Insufficient cash to buy deposit.");
    }
    refresh();
}

void MainWindow::addStockDialog() {
    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Add Stock", "Amount:", 10000.0, 0.0, 1e12, 2, &ok);
    if (!ok) return;

    const auto &comps = gc_->market().companies();
    QStringList names;
    for (const auto &c : comps) names << c.name;
    if (names.empty()) {
        QMessageBox::warning(this, "No companies", "Add companies in Settings first.");
        return;
    }
    QString name = QInputDialog::getItem(this, "Add Stock", "Company:", names, 0, false, &ok);
    if (!ok) return;

    if (!gc_->fund().buyStock(name, amount, gc_->market())) {
        QMessageBox::warning(this, "Not enough cash", "Insufficient cash to buy stock.");
    }
    refresh();
}

/* ---------------- Refreshers ---------------- */
void MainWindow::refresh() {
    const auto &m = gc_->market();

    lblMonth_->setText(QString("%1 / %2").arg(gc_->month()).arg(gc_->totalMonths()));
    lblTaxRate_->setText(QString::number(gc_->taxRate() * 100.0, 'f', 2) + "%");
    lblDepRate_->setText(QString::number(m.current().depositRate * 100.0, 'f', 2) + "%");

    double eq = gc_->fund().equity(m);
    lblCapital_->setText(QString::number(eq, 'f', 2));
    lblCash_->setText(QString::number(gc_->fund().cash(), 'f', 2));
    lblLastProfit_->setText(QString::number(gc_->lastMonthProfit(), 'f', 2));

    // Portfolio table
    const auto &items = gc_->fund().portfolio().items();
    table_->setRowCount((int) items.size());
    for (int i = 0; i < (int) items.size(); ++i) {
        auto inv = items[i];
        table_->setItem(i, 0, new QTableWidgetItem(inv->type()));
        table_->setItem(i, 1, new QTableWidgetItem(inv->name()));
        table_->setItem(i, 2, new QTableWidgetItem(QString::number(inv->marketValue(m), 'f', 2)));
        QString notes;
        if (inv->type() == "Deposit") notes = "monthly interest";
        else if (inv->type() == "Stock") notes = "mark-to-market";
        else if (inv->type() == "Currency") notes = "monthly yield";
        else if (inv->type() == "Bond") notes = "monthly yield";
        else if (inv->type() == "Metal") notes = "mark-to-market";
        table_->setItem(i, 3, new QTableWidgetItem(notes));
    }
    table_->resizeColumnsToContents();

    // Stock prices table with Δ
    const auto &companies = m.companies();
    tblStockPrices_->setRowCount((int)companies.size());
    for (int i = 0; i < (int)companies.size(); ++i) {
        const auto &c = companies[i];
        tblStockPrices_->setItem(i, 0, new QTableWidgetItem(c.name));
        tblStockPrices_->setItem(i, 1, new QTableWidgetItem(QString::number(c.price, 'f', 2)));

        double lastPrice = 1.0;
        if (m.lastPrices().size() > (size_t)i) lastPrice = m.lastPrices()[i];
        double delta = c.price - lastPrice;
        auto *itemDelta = new QTableWidgetItem(QString::number(delta, 'f', 2));
        if (delta > 0) itemDelta->setForeground(QBrush(Qt::green));
        else if (delta < 0) itemDelta->setForeground(QBrush(Qt::red));
        tblStockPrices_->setItem(i, 2, itemDelta);
    }
    tblStockPrices_->resizeColumnsToContents();

    refreshCompaniesTable();
    refreshAssetTables();
}

void MainWindow::refreshCompaniesTable() {
    const auto &comps = gc_->market().companies();
    tblCompanies_->setRowCount((int)comps.size());
    for (int i = 0; i < (int)comps.size(); ++i) {
        tblCompanies_->setItem(i, 0, new QTableWidgetItem(comps[i].name));
        tblCompanies_->setItem(i, 1, new QTableWidgetItem(QString::number(comps[i].price, 'f', 2)));
    }
    tblCompanies_->resizeColumnsToContents();
}

void MainWindow::refreshAssetTables() {
    const auto &currs = gc_->market().currencies();
    tblCurrencies_->setRowCount((int)currs.size());
    for (int i = 0; i < (int)currs.size(); ++i) {
        tblCurrencies_->setItem(i, 0, new QTableWidgetItem(currs[i].name));
        tblCurrencies_->setItem(i, 1, new QTableWidgetItem(QString::number(currs[i].rate, 'f', 4)));
    }
    tblCurrencies_->resizeColumnsToContents();

    const auto &bonds = gc_->market().bonds();
    tblBonds_->setRowCount((int)bonds.size());
    for (int i = 0; i < (int)bonds.size(); ++i) {
        tblBonds_->setItem(i, 0, new QTableWidgetItem(bonds[i].name));
        tblBonds_->setItem(i, 1, new QTableWidgetItem(QString::number(bonds[i].annualYield, 'f', 4)));
    }
    tblBonds_->resizeColumnsToContents();

    const auto &metals = gc_->market().metals();
    tblMetals_->setRowCount((int)metals.size());
    for (int i = 0; i < (int)metals.size(); ++i) {
        tblMetals_->setItem(i, 0, new QTableWidgetItem(metals[i].name));
        tblMetals_->setItem(i, 1, new QTableWidgetItem(QString::number(metals[i].price, 'f', 2)));
    }
    tblMetals_->resizeColumnsToContents();
}

/* ---------------- Settings slots ---------------- */
void MainWindow::onAddCompany() {
    QString name = editCompanyName_->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Empty name", "Please enter company name.");
        return;
    }
    double price = spinCompanyPrice_->value();
    gc_->market().addCompany(name, price);
    editCompanyName_->clear();
    spinCompanyPrice_->setValue(100.0);
    refresh();
}

void MainWindow::onRemoveCompany() {
    int sel = tblCompanies_->currentRow();
    if (sel < 0) return;
    gc_->market().removeCompanyAt(sel);
    refresh();
}

void MainWindow::onAddCurrency() {
    bool ok = false;
    QString name = QInputDialog::getText(this, "Add Currency", "Name:", QLineEdit::Normal, "USD", &ok);
    if (!ok || name.isEmpty()) return;
    double rate = QInputDialog::getDouble(this, "Add Currency", "Rate:", 1.0, 0.0, 1e9, 4, &ok);
    if (!ok) return;
    gc_->market().addCurrency(name, rate);
    refresh();
}

void MainWindow::onRemoveCurrency() {
    int sel = tblCurrencies_->currentRow();
    if (sel < 0) return;
    gc_->market().removeCurrencyAt(sel);
    refresh();
}

void MainWindow::onAddBond() {
    bool ok = false;
    QString name = QInputDialog::getText(this, "Add Bond", "Name:", QLineEdit::Normal, "Gov Bond", &ok);
    if (!ok || name.isEmpty()) return;
    double y = QInputDialog::getDouble(this, "Add Bond", "Annual yield:", 0.05, 0.0, 1.0, 4, &ok);
    if (!ok) return;
    gc_->market().addBond(name, y);
    refresh();
}

void MainWindow::onRemoveBond() {
    int sel = tblBonds_->currentRow();
    if (sel < 0) return;
    gc_->market().removeBondAt(sel);
    refresh();
}

void MainWindow::onAddMetal() {
    bool ok = false;
    QString name = QInputDialog::getText(this, "Add Metal", "Name:", QLineEdit::Normal, "Gold", &ok);
    if (!ok || name.isEmpty()) return;
    double price = QInputDialog::getDouble(this, "Add Metal", "Price:", 50.0, 0.01, 1e9, 2, &ok);
    if (!ok) return;
    gc_->market().addMetal(name, price);
    refresh();
}

void MainWindow::onRemoveMetal() {
    int sel = tblMetals_->currentRow();
    if (sel < 0) return;
    gc_->market().removeMetalAt(sel);
    refresh();
}

/* ---------------- Simulation quick-buy slots ---------------- */
void MainWindow::onAddCurrencySim() {
    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Buy Currency", "Amount:", 10000.0, 0.0, 1e12, 2, &ok);
    if (!ok) return;
    // choose currency
    QStringList names;
    for (const auto &c : gc_->market().currencies()) names << c.name;
    if (names.empty()) { QMessageBox::warning(this, "No currencies", "Add currencies in Settings first."); return; }
    QString name = QInputDialog::getItem(this, "Buy Currency", "Currency:", names, 0, false, &ok);
    if (!ok) return;
    double rate = 1.0;
    // use provided rate from market if needed (InvestmentFund::buyCurrency currently expects monthlyRate)
    double monthlyRate = 0.0;
    if (!gc_->fund().buyCurrency(name, amount, monthlyRate)) QMessageBox::warning(this, "Not enough cash", "Insufficient cash.");
    refresh();
}

void MainWindow::onAddBondSim() {
    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Buy Bond", "Amount:", 10000.0, 0.0, 1e12, 2, &ok);
    if (!ok) return;
    QStringList names;
    for (const auto &b : gc_->market().bonds()) names << b.name;
    if (names.empty()) { QMessageBox::warning(this, "No bonds", "Add bonds in Settings first."); return; }
    QString name = QInputDialog::getItem(this, "Buy Bond", "Bond:", names, 0, false, &ok);
    if (!ok) return;
    // find yield
    double yield = 0.0;
    for (const auto &b : gc_->market().bonds()) if (b.name == name) { yield = b.annualYield; break; }
    if (!gc_->fund().buyBond(name, amount, yield)) QMessageBox::warning(this, "Not enough cash", "Insufficient cash.");
    refresh();
}

void MainWindow::onAddMetalSim() {
    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Buy Metal", "Amount (units):", 100.0, 0.0, 1e9, 2, &ok);
    if (!ok) return;
    QStringList names;
    for (const auto &m : gc_->market().metals()) names << m.name;
    if (names.empty()) { QMessageBox::warning(this, "No metals", "Add metals in Settings first."); return; }
    QString name = QInputDialog::getItem(this, "Buy Metal", "Metal:", names, 0, false, &ok);
    if (!ok) return;
    if (!gc_->fund().buyMetal(name, amount, gc_->market())) QMessageBox::warning(this, "Not enough cash", "Insufficient cash.");
    refresh();
}

/* ---------------- Apply settings ---------------- */
void MainWindow::onApplySettings() {
    // Apply simulation params
    gc_->setTotalMonths(spinTotalMonths_->value());
    gc_->fund().setCash(spinInitialCapital_->value());
    gc_->setTaxRate(spinTaxRate_->value());
    gc_->market().setDepositRate(spinDepRate_->value());
    gc_->resetMonthCounter();

    // Apply currencies table edits (rates)
    for (int i = 0; i < tblCurrencies_->rowCount(); ++i) {
        auto *it = tblCurrencies_->item(i, 1);
        if (!it) continue;
        double rate = it->text().toDouble();
        gc_->market().setCurrencyRate(i, rate);
    }

    // Apply bonds table edits (yields)
    for (int i = 0; i < tblBonds_->rowCount(); ++i) {
        auto *it = tblBonds_->item(i, 1);
        if (!it) continue;
        double y = it->text().toDouble();
        gc_->market().setBondYield(i, y);
    }

    // Apply metals table edits (prices)
    for (int i = 0; i < tblMetals_->rowCount(); ++i) {
        auto *it = tblMetals_->item(i, 1);
        if (!it) continue;
        double p = it->text().toDouble();
        gc_->market().setMetalPrice(i, p);
    }

    refresh();
    QMessageBox::information(this, "Settings applied", "Simulation parameters applied.");
}