#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QRandomGenerator>

using namespace std;

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

    // === ЛОГИКА ИГР ===
    // Числовая последовательность (page1)
    void on_btnSeqSubmit_clicked();     // Кнопка "Ответить"
    void generateSequence();             // Генерация новой последовательности
    void updateSeqTimer();               // Обновление таймера

    // === СИСТЕМА ОЧКОВ ===
    void addScore(int points);           // Добавление очков
    void updateCombo(bool correct);      // Обновление комбо
    void adjustDifficulty();             // Корректировка сложности

private:
    Ui::MainWindow *ui;

    // === ПЕРЕМЕННЫЕ СОСТОЯНИЯ ===
    int m_score = 0;          // Текущие очки
    int m_combo = 0;          // Счетчик комбо
    int m_difficulty = 1;     // Уровень сложности (1-3)

    // === ДАННЫЕ ТЕКУЩЕЙ ИГРЫ ===
    int m_correctAnswer = 0;  // Правильный ответ для sequence
    QTimer *m_seqTimer;       // Таймер для последовательности
    int m_timeLeft = 0;       // Оставшееся время

    // === ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ===
    void updateAllStatsLabels();         // Обновление всех лейблов статистики
    void returnToMainMenu();             // Возврат в главное меню
};
#endif // MAINWINDOW_H
