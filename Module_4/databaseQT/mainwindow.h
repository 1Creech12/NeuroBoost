#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databaseqt.h"  // Подключаем ваш класс БД

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Здесь можно добавить обработчики кнопок


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    PlayerDatabase *m_playerDb;

    void refreshTable();  // Обновить таблицу
    void clearInputs();   // Очистить поля ввода
};

#endif // MAINWINDOW_H