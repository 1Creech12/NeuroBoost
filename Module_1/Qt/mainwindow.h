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
    void on_lineEdit_returnPressed(); // Единственный обработчик

private:
    Ui::MainWindow *ui;

    bool isProcessing; // Защита от двойных нажатий
    int currentA, currentB;
    char currentOp;
    int nums[4];
    char ops[3];
    int difficulty;
    int problemsSolved;
    bool isComplexMode;
    bool gameEnded;

    void generateProblem();
    int evaluateComplex();
};

#endif // MAINWINDOW_H