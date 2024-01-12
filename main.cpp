#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto mainWindow = new MainWindow(nullptr, &a);
    mainWindow->show();
    return a.exec();
}
