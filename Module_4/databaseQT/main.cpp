#include <QApplication>
#include "mainwindow.h"      // ← Подключаем ГЛАВНОЕ ОКНО, а не databaseqt.h!

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;   // ← Создаём ГЛАВНОЕ ОКНО (MainWindow, а не databaseqt!)
    w.show();       // ← Показываем окно

    return a.exec(); // ← Запускаем цикл событий
}