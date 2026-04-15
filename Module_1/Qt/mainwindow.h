#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_clicked();
    void on_pushButtonNext_clicked();
    void on_lineEdit_returnPressed(); // Слот для обработки нажатия Enter

private:
    Ui::MainWindow *ui;

    // Члены класса для хранения состояния текущего примера
    int currentA;
    int currentB;
    char currentOp;
    int difficulty;

    void generateProblem(); // Вспомогательная функция генерации
};

#endif // MAINWINDOW_H