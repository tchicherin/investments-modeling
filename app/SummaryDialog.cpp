#include "SummaryDialog.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QHeaderView>
#include <QMessageBox>

SummaryDialog::SummaryDialog(const GameController::SummaryStats &stats,
                             const std::vector<MonthResult> &history,
                             QWidget *parent)
  : QDialog(parent), stats_(stats), history_(history) {
  setWindowTitle("Simulation Summary");
  setMinimumSize(800, 600);

  lblSummary_ = new QLabel(this);
  lblSummary_->setWordWrap(true);

  tblHistory_ = new QTableWidget(this);
  tblHistory_->setColumnCount(6);
  QStringList headers{"Month", "Equity Before", "Portfolio Profit", "Tax Paid", "Flow", "Equity After"};
  tblHistory_->setHorizontalHeaderLabels(headers);
  tblHistory_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tblHistory_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  btnExport_ = new QPushButton("Export CSV", this);
  btnClose_ = new QPushButton("Close", this);

  auto *btnLayout = new QHBoxLayout();
  btnLayout->addStretch();
  btnLayout->addWidget(btnExport_);
  btnLayout->addWidget(btnClose_);

  auto *layout = new QVBoxLayout(this);
  layout->addWidget(lblSummary_);
  layout->addWidget(tblHistory_);
  layout->addLayout(btnLayout);
  setLayout(layout);

  populateSummary();
  populateHistoryTable();

  connect(btnExport_, &QPushButton::clicked, this, &SummaryDialog::onExportCsv);
  connect(btnClose_, &QPushButton::clicked, this, &SummaryDialog::onClose);
}

void SummaryDialog::populateSummary() {
  const auto &s = stats_;
  QString txt;
  txt += QString("<b>Initial equity:</b> %1<br>").arg(s.initialEquity, 0, 'f', 2);
  txt += QString("<b>Final equity:</b> %1<br>").arg(s.finalEquity, 0, 'f', 2);
  txt += QString("<b>Total return:</b> %1 %<br>").arg(s.totalReturn * 100.0, 0, 'f', 2);
  txt += QString("<b>Annualized return (approx):</b> %1 %<br>").arg(s.annualizedReturn * 100.0, 0, 'f', 2);
  txt += QString("<b>Average monthly return:</b> %1 %<br>").arg(s.avgMonthlyReturn * 100.0, 0, 'f', 3);
  txt += QString("<b>Monthly volatility:</b> %1 %<br>").arg(s.volatilityMonthly * 100.0, 0, 'f', 3);
  txt += QString("<b>Percent profitable months:</b> %1 %<br>").arg(s.percentProfitableMonths, 0, 'f', 2);
  txt += QString("<b>Max drawdown:</b> %1 %<br>").arg(s.maxDrawdown * 100.0, 0, 'f', 2);
  txt += QString("<b>Total inflow (sum of flows):</b> %1<br>").arg(s.totalInflow, 0, 'f', 2);
  txt += QString("<b>Total tax paid:</b> %1<br>").arg(s.totalTaxPaid, 0, 'f', 2);

  lblSummary_->setText(txt);
}

void SummaryDialog::populateHistoryTable() {
  tblHistory_->setRowCount(static_cast<int>(history_.size()));
  for (int i = 0; i < static_cast<int>(history_.size()); ++i) {
    const MonthResult &m = history_[i];
    tblHistory_->setItem(i, 0, new QTableWidgetItem(QString::number(m.monthIndex + 1)));
    tblHistory_->setItem(i, 1, new QTableWidgetItem(QString::number(m.equityBefore, 'f', 2)));
    tblHistory_->setItem(i, 2, new QTableWidgetItem(QString::number(m.portfolioProfit, 'f', 2)));
    tblHistory_->setItem(i, 3, new QTableWidgetItem(QString::number(m.taxPaid, 'f', 2)));
    tblHistory_->setItem(i, 4, new QTableWidgetItem(QString::number(m.flow, 'f', 2)));
    tblHistory_->setItem(i, 5, new QTableWidgetItem(QString::number(m.equityAfter, 'f', 2)));
  }
  tblHistory_->resizeColumnsToContents();
}

void SummaryDialog::onExportCsv() {
  QString path = QFileDialog::getSaveFileName(this, "Export history to CSV", "simulation_history.csv", "CSV files (*.csv)");
  if (path.isEmpty()) return;

  QFile f(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Error", QString("Cannot open file: %1").arg(path));
    return;
  }
  QTextStream out(&f);
  // header
  out << "Month,EquityBefore,PortfolioProfit,TaxPaid,Flow,EquityAfter\n";
  for (const MonthResult &m : history_) {
    out << (m.monthIndex + 1) << ",";
    out << QString::number(m.equityBefore, 'f', 6) << ",";
    out << QString::number(m.portfolioProfit, 'f', 6) << ",";
    out << QString::number(m.taxPaid, 'f', 6) << ",";
    out << QString::number(m.flow, 'f', 6) << ",";
    out << QString::number(m.equityAfter, 'f', 6) << "\n";
  }
  f.close();
  QMessageBox::information(this, "Exported", QString("History exported to %1").arg(path));
}

void SummaryDialog::onClose() {
  accept();
}
