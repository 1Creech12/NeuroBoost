#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QMetaObject>
#include "Timer.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct ExcessObject
{
    QString name;
    QString category;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // === НАВИГАЦИЯ ===
    void on_sequenceBtn_clicked();      // Кнопка "Числовая последовательность"
    void on_excessBtn_clicked();        // Кнопка "Лишний объект"
    void on_compareBtn_clicked();       // Кнопка "Сравнить выражения"
    void on_statermentsBtn_clicked();   // Кнопка "Правильное утверждение"
    void on_memoryBtn_clicked();        // Кнопка "Сравни символы"
    void on_randomPlayBtn_clicked();    // Кнопка "Рандомная игра"
    void on_BtnSeqBack_clicked();       // Кнопка "Назад" на странице последовательности
    void on_btnExcessBack_clicked();    // Назад
    void on_btnCompareBack_clicked();   // Назад
    void on_btnStatermentBack_clicked();// Назад
    void on_btnMemoryBack_clicked();    // Назад

    // === ЛОГИКА ИГР ===
    // Числовая последовательность (page1)
    void on_btnSeqSubmit_clicked();     // Кнопка "Ответить"
    void generateSequence();             // Генерация новой последовательности

    // Excess
    void on_btnOpt1_clicked();
    void on_btnOpt2_clicked();
    void on_btnOpt3_clicked();
    void on_btnOpt4_clicked();
    void generateExcess();

    // Compare
    void on_btnGreater_clicked();
    void on_btnLess_clicked();
    void on_btnEqual_clicked();
    void generateCompare();

    // Statements
    void on_btnTrue_clicked();
    void on_btnFalse_clicked();
    void generateStatement();

    // Memory
    void on_btnMemorySubmit_clicked();
    void generateMemory();

    // === СИСТЕМА ОЧКОВ ===
    void addScore(int points);           // Добавление очков
    void updateCombo(bool correct);      // Обновление комбо
    void adjustDifficulty();             // Корректировка сложности
    void deductScore(int points);        // Отнимание очков


private:
    Ui::MainWindow *ui;

    int m_score = 0;          // Текущие очки
    int m_combo = 0;          // Счетчик комбо
    int m_difficulty = 1;     // Уровень сложности (1-3)
    int bestScore = 0;
    int m_currentGame = 0;
    //int add_point = 10;
    int add_point_Seq = 10;
    int add_point_Excess = 10;
    int add_point_Compare = 10;
    int add_point_Statement = 10;
    int add_point_Memory = 10;
    //int ded_point = 5;
    int ded_point_Seq = 5;
    int ded_point_Excess = 5;
    int ded_point_Compare = 5;
    int ded_point_Statement = 5;
    int ded_point_Memory = 5;

    // Данные игр
    int m_correctAnswer = 0;  // Правильный ответ для sequence
    Timer *m_seqTimer;    // Таймеры
    Timer *m_excessTimer;
    Timer *m_compareTimer;
    Timer *m_statementTimer;
    Timer *m_memoryTimer;
    int m_timeLimit = 0;       // Оставшееся время
    bool m_gameActive = false;
    int m_correctOption = 0;
    QString m_correctSymbols;
    bool m_correctStatement = false;
    char m_correctCompare = '=';
    vector<ExcessObject> m_currentObjects;

    // === ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ===
    void updateAllStatsLabels();         // Обновление всех лейблов статистики
    void returnToMainMenu();             // Возврат в главное меню
    void startTimerMonitoring();
    void stopAllTimers();
    void startGameTimer(int gameType);
};
#endif // MAINWINDOW_H
