#include "settings.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings w;
    return a.exec();
}
