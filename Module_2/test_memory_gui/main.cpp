#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainGames w;
    w.show();
    return QCoreApplication::exec();
}
