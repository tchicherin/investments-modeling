#ifndef SUMMARYDIALOG_H
#define SUMMARYDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "GameController.h" // для GameController::SummaryStats и MonthResult

class SummaryDialog : public QDialog {
  Q_OBJECT
 public:
  explicit SummaryDialog(const GameController::SummaryStats &stats,
                         const std::vector<MonthResult> &history,
                         QWidget *parent = nullptr);

 private slots:
  void onExportCsv();
  void onClose();

 private:
  const GameController::SummaryStats stats_;
  const std::vector<MonthResult> history_;

  QLabel *lblSummary_;
  QTableWidget *tblHistory_;
  QPushButton *btnExport_;
  QPushButton *btnClose_;

  void populateSummary();
  void populateHistoryTable();
};

#endif // SUMMARYDIALOG_H
