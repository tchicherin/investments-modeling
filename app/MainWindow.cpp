#include "MainWindow.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>

#include <cmath>

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

  // --- Simulation tab
  auto *simTab = new QWidget(this);
  setupSimulationTab(simTab);
  tabs_->addTab(simTab, "Simulation");

  // --- Settings tab
  auto *settingsTab = new QWidget(this);
  setupSettingsTab(settingsTab);
  tabs_->addTab(settingsTab, "Settings");

  auto *layout = new QVBoxLayout();
  layout->addWidget(tabs_);
  central->setLayout(layout);
}

void MainWindow::setupSimulationTab(QWidget *w) {
  // create widgets
  lblMonth_ = new QLabel(this);
  lblCapital_ = new QLabel(this);
  lblCash_ = new QLabel(this);
  lblLastProfit_ = new QLabel(this);
  lblTaxRate_ = new QLabel(this);
  lblStockPrice_ = new QLabel(this);
  lblStockPrice_->setWordWrap(true);
  lblDepRate_ = new QLabel(this);

  table_ = new QTableWidget(this);
  table_->setColumnCount(4);
  QStringList headers{"Type", "Name", "Value", "Notes"};
  table_->setHorizontalHeaderLabels(headers);
  table_->setSelectionBehavior(QAbstractItemView::SelectRows);
  table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  btnNext_ = new QPushButton("▶ Next Month", this);
  btnAddDep_ = new QPushButton("➕ Add Deposit", this);
  btnAddStock_ = new QPushButton("➕ Add Stock", this);
  btnSell_ = new QPushButton("✖ Sell Selected", this);

  auto *topInfo = new QFormLayout();
  topInfo->addRow("Month:", lblMonth_);
  topInfo->addRow("Tax rate:", lblTaxRate_);
  topInfo->addRow("Stock prices:", lblStockPrice_);
  topInfo->addRow("Deposit rate:", lblDepRate_);
  topInfo->addRow("Total equity:", lblCapital_);
  topInfo->addRow("Cash:", lblCash_);
  topInfo->addRow("Last month profit:", lblLastProfit_);

  auto *btns = new QHBoxLayout();
  btns->addWidget(btnNext_);
  btns->addStretch();
  btns->addWidget(btnAddDep_);
  btns->addWidget(btnAddStock_);
  btns->addWidget(btnSell_);

  auto *layout = new QVBoxLayout();
  layout->addLayout(topInfo);
  layout->addWidget(table_);
  layout->addLayout(btns);

  w->setLayout(layout);

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
}

void MainWindow::setupSettingsTab(QWidget *w) {
  // Left: simulation parameters
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

  auto *form = new QFormLayout();
  form->addRow("Total months:", spinTotalMonths_);
  form->addRow("Initial capital:", spinInitialCapital_);
  form->addRow("Tax rate:", spinTaxRate_);
  form->addRow("Deposit rate:", spinDepRate_);

  // Right: companies table + controls
  tblCompanies_ = new QTableWidget(this);
  tblCompanies_->setColumnCount(2);
  tblCompanies_->setHorizontalHeaderLabels(QStringList{"Company", "Price"});
  tblCompanies_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tblCompanies_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tblCompanies_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  editCompanyName_ = new QLineEdit(this);
  spinCompanyPrice_ = new QDoubleSpinBox(this);
  spinCompanyPrice_->setRange(0.01, 1e9);
  spinCompanyPrice_->setDecimals(2);
  spinCompanyPrice_->setValue(100.0);

  btnAddCompany_ = new QPushButton("Add Company", this);
  btnRemoveCompany_ = new QPushButton("Remove Selected", this);
  btnApplySettings_ = new QPushButton("Apply Settings", this);

  auto *companyControls = new QHBoxLayout();
  companyControls->addWidget(editCompanyName_);
  companyControls->addWidget(spinCompanyPrice_);
  companyControls->addWidget(btnAddCompany_);
  companyControls->addWidget(btnRemoveCompany_);

  auto *rightLayout = new QVBoxLayout();
  rightLayout->addWidget(tblCompanies_);
  rightLayout->addLayout(companyControls);
  rightLayout->addStretch();
  rightLayout->addWidget(btnApplySettings_);

  // Combine left and right
  auto *h = new QHBoxLayout();
  auto *leftWidget = new QWidget(this);
  leftWidget->setLayout(form);
  h->addWidget(leftWidget, 0);
  auto *rightWidget = new QWidget(this);
  rightWidget->setLayout(rightLayout);
  h->addWidget(rightWidget, 1);

  w->setLayout(h);

  // fill companies table from market
  const auto &comps = gc_->market().companies();
  tblCompanies_->setRowCount(static_cast<int>(comps.size()));
  for (int i = 0; i < static_cast<int>(comps.size()); ++i) {
    auto *c0 = new QTableWidgetItem(comps[i].name);
    auto *c1 = new QTableWidgetItem(QString::number(comps[i].price, 'f', 2));
    tblCompanies_->setItem(i, 0, c0);
    tblCompanies_->setItem(i, 1, c1);
  }

  // connections
  connect(btnAddCompany_, &QPushButton::clicked, this, &MainWindow::onAddCompany);
  connect(btnRemoveCompany_, &QPushButton::clicked, this, &MainWindow::onRemoveCompany);
  connect(btnApplySettings_, &QPushButton::clicked, this, &MainWindow::onApplySettings);
}

void MainWindow::refresh() {
  const auto &m = gc_->market().current();
  lblMonth_->setText(QString("%1 / %2").arg(gc_->month()).arg(gc_->totalMonths()));
  lblTaxRate_->setText(QString::number(gc_->taxRate() * 100.0, 'f', 2) + "%");

  // Show all companies and prices in a single label
  const auto &comps = gc_->market().companies();
  if (!comps.empty()) {
    QStringList parts;
    parts.reserve(static_cast<int>(comps.size()));
    for (const auto &s : comps) {
      parts << QString("%1: %2").arg(s.name, QString::number(s.price, 'f', 2));
    }
    lblStockPrice_->setText(parts.join("; "));
  } else {
    // fallback to aggregate single price
    lblStockPrice_->setText(QString::number(m.stockPrice, 'f', 2));
  }

  lblDepRate_->setText(QString::number(m.depositRate * 100.0, 'f', 2) + "%");

  double eq = gc_->fund().equity(gc_->market());
  lblCapital_->setText(QString::number(eq, 'f', 2));
  lblCash_->setText(QString::number(gc_->fund().cash(), 'f', 2));
  lblLastProfit_->setText(QString::number(gc_->lastMonthProfit(), 'f', 2));

  // Таблица активов
  const auto &items = gc_->fund().portfolio().items();
  table_->setRowCount((int) items.size());
  for (int i = 0; i < (int) items.size(); ++i) {
    auto inv = items[i];
    auto *c0 = new QTableWidgetItem(inv->type());
    auto *c1 = new QTableWidgetItem(inv->name());
    auto val = inv->marketValue(gc_->market());
    auto *c2 = new QTableWidgetItem(QString::number(val, 'f', 2));
    QString notes = (inv->type() == "Deposit") ? "monthly interest" : "mark-to-market";
    auto *c3 = new QTableWidgetItem(notes);

    table_->setItem(i, 0, c0);
    table_->setItem(i, 1, c1);
    table_->setItem(i, 2, c2);
    table_->setItem(i, 3, c3);
  }
  table_->resizeColumnsToContents();

  // Update settings tab values in case they were changed elsewhere
  spinTotalMonths_->setValue(gc_->totalMonths());
  spinInitialCapital_->setValue(gc_->fund().cash());
  spinTaxRate_->setValue(gc_->taxRate());
  spinDepRate_->setValue(gc_->market().current().depositRate);

  // refresh companies table
  const auto &comps2 = gc_->market().companies();
  tblCompanies_->setRowCount(static_cast<int>(comps2.size()));
  for (int i = 0; i < static_cast<int>(comps2.size()); ++i) {
    auto *c0 = new QTableWidgetItem(comps2[i].name);
    auto *c1 = new QTableWidgetItem(QString::number(comps2[i].price, 'f', 2));
    tblCompanies_->setItem(i, 0, c0);
    tblCompanies_->setItem(i, 1, c1);
  }
  tblCompanies_->resizeColumnsToContents();
}

void MainWindow::addDepositDialog() {
  bool ok = false;
  double amount = QInputDialog::getDouble(this, "Add Deposit", "Amount:", 10000.0, 0.0, 1e12, 2, &ok);
  if (!ok) return;
  double rate = QInputDialog::getDouble(this, "Add Deposit", "Annual rate (e.g. 0.08):", 0.08, 0.001, 1.0, 3, &ok);
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
  // choose company from existing companies
  const auto &comps = gc_->market().companies();
  QStringList names;
  for (const auto &c : comps) names << c.name;
  if (names.empty()) {
    QMessageBox::warning(this, "No companies", "There are no companies in the market. Add companies in Settings first.");
    return;
  }
  QString name = QInputDialog::getItem(this, "Add Stock", "Company:", names, 0, false, &ok);
  if (!ok) return;

  if (!gc_->fund().buyStock(name, amount, gc_->market())) {
    QMessageBox::warning(this, "Not enough cash", "Insufficient cash to buy stock.");
  }
  refresh();
}

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

void MainWindow::onApplySettings() {
  // Apply total months, initial capital, tax rate, deposit rate
  gc_->setTotalMonths(spinTotalMonths_->value());
  gc_->fund().setCash(spinInitialCapital_->value());
  gc_->setTaxRate(spinTaxRate_->value());
  gc_->market().setDepositRate(spinDepRate_->value());

  // reset month counter (user changed scenario)
  gc_->resetMonthCounter();

  QMessageBox::information(this, "Settings applied", "Simulation parameters applied.");
  refresh();
}
