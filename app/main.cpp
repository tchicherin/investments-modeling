#include <QApplication>
#include "MainWindow.h"
#include "GameController.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Параметры игры: стартовый капитал, число месяцев, налог
    auto* gc = new GameController(/*initialCapital*/ 560000.0,
                                  /*totalMonths*/ 24,
                                  /*taxRate*/ 0.17);

    MainWindow w(gc);
    w.resize(900, 600);
    w.show();

    return app.exec();
}
