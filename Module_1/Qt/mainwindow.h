#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QElapsedTimer>

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
    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    bool isProcessing; // Защита от спама Enter
    QElapsedTimer answerTimer; // ⏱ Точный замер без изменения твоего Timer

    // Простой режим
    int currentA, currentB;
    char currentOp;
    // Сложный режим
    int nums[4];
    char ops[3];

    int difficulty;
    int problemsSolved;
    bool isComplexMode;
    bool gameEnded;

    // 📊 Статистика времени (в миллисекундах)
    qint64 totalTimeMs;
    qint64 fastestTimeMs;
    qint64 slowestTimeMs;

    void generateProblem();
    int evaluateComplex();
    void showFinalResults();
};
#endif // MAINWINDOW_H