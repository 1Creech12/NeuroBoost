#ifndef MAINMODULE_1_H
#define MAINMODULE_1_H
#include <QMainWindow>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainModule_1; }
QT_END_NAMESPACE

class MainModule_1 : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainModule_1(QWidget *parent = nullptr);
    ~MainModule_1() override;

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();

private:
    Ui::MainModule_1 *ui;
    bool isProcessing;
    QElapsedTimer answerTimer;

    int currentA, currentB;
    char currentOp;
    int nums[4];
    char ops[3];

    int difficulty;
    int problemsSolved;
    bool isComplexMode;
    bool gameEnded;

    qint64 totalTimeMs;
    qint64 fastestTimeMs;
    qint64 slowestTimeMs;

    // 🎯 Переменные для статистики
    int correctAnswers;
    int currentScore; // Очки

    void generateProblem();
    int evaluateComplex();
    void showFinalResults();
};
#endif // MAINMODULE_1_H
