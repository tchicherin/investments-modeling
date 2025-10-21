#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFormLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(GameController* controller, QWidget* parent)
    : QMainWindow(parent), gc_(controller) {
    setupUi();
    refresh();
    connect(gc_, &GameController::updated, this, &MainWindow::refresh);
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    setCentralWidget(central);
    setWindowTitle("Investment Portfolio Simulator");

    lblMonth_ = new QLabel(this);
    lblCapital_ = new QLabel(this);
    lblCash_ = new QLabel(this);
    lblLastProfit_ = new QLabel(this);
    lblTaxRate_ = new QLabel(this);
    lblStockPrice_ = new QLabel(this);
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

    auto* topInfo = new QFormLayout();
    topInfo->addRow("Month:", lblMonth_);
    topInfo->addRow("Tax rate:", lblTaxRate_);
    topInfo->addRow("Stock price:", lblStockPrice_);
    topInfo->addRow("Deposit rate:", lblDepRate_);
    topInfo->addRow("Total equity:", lblCapital_);
    topInfo->addRow("Cash:", lblCash_);
    topInfo->addRow("Last month profit:", lblLastProfit_);

    auto* btns = new QHBoxLayout();
    btns->addWidget(btnNext_);
    btns->addStretch();
    btns->addWidget(btnAddDep_);
    btns->addWidget(btnAddStock_);
    btns->addWidget(btnSell_);

    auto* layout = new QVBoxLayout();
    layout->addLayout(topInfo);
    layout->addWidget(table_);
    layout->addLayout(btns);

    central->setLayout(layout);

    // connections
    connect(btnNext_, &QPushButton::clicked, gc_, &GameController::nextMonth);
    connect(btnAddDep_, &QPushButton::clicked, this, &MainWindow::addDepositDialog);
    connect(btnAddStock_, &QPushButton::clicked, this, &MainWindow::addStockDialog);
    connect(btnSell_, &QPushButton::clicked, [this](){
        int sel = table_->currentRow();
        if (sel < 0) return;
        gc_->fund().sellInvestment(sel, gc_->market());
        refresh();
    });
}

void MainWindow::refresh() {
    const auto& m = gc_->market().current();
    lblMonth_->setText(QString("%1 / %2").arg(gc_->month()).arg(gc_->totalMonths()));
    lblTaxRate_->setText(QString::number(gc_->taxRate() * 100.0, 'f', 2) + "%");
    lblStockPrice_->setText(QString::number(m.stockPrice, 'f', 2));
    lblDepRate_->setText(QString::number(m.depositRate * 100.0, 'f', 2) + "%");

    double eq = gc_->fund().equity(gc_->market());
    lblCapital_->setText(QString::number(eq, 'f', 2));
    lblCash_->setText(QString::number(gc_->fund().cash(), 'f', 2));
    lblLastProfit_->setText(QString::number(gc_->lastMonthProfit(), 'f', 2));

    // Таблица активов
    const auto& items = gc_->fund().portfolio().items();
    table_->setRowCount((int)items.size());
    for (int i = 0; i < (int)items.size(); ++i) {
        auto inv = items[i];
        auto* c0 = new QTableWidgetItem(inv->type());
        auto* c1 = new QTableWidgetItem(inv->name());
        auto val = inv->marketValue(gc_->market());
        auto* c2 = new QTableWidgetItem(QString::number(val, 'f', 2));
        QString notes = (inv->type() == "Deposit") ? "monthly interest" : "mark-to-market";
        auto* c3 = new QTableWidgetItem(notes);

        table_->setItem(i, 0, c0);
        table_->setItem(i, 1, c1);
        table_->setItem(i, 2, c2);
        table_->setItem(i, 3, c3);
    }
    table_->resizeColumnsToContents();
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
    QString name = QInputDialog::getText(this, "Add Stock", "Name:", QLineEdit::Normal, "Index Fund", &ok);
    if (!ok) return;

    if (!gc_->fund().buyStock(name, amount, gc_->market())) {
        QMessageBox::warning(this, "Not enough cash", "Insufficient cash to buy stock.");
    }
    refresh();
}
