#include "colormemory.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorMemory w;
    w.show();
    return QCoreApplication::exec();
}
