#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_seqTimer = new QTimer(this);
    connect(m_seqTimer, &QTimer::timeout, this, &MainWindow::updateSeqTimer);

    updateAllStatsLabels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sequenceBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    generateSequence();

    // Запуск таймера (время зависит от сложности)
    m_timeLeft = 30 - (m_difficulty * 3);
    if (m_timeLeft < 10) m_timeLeft = 10;
    ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLeft) + "с");
    m_seqTimer->start(1000);

}


void MainWindow::on_excessBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_compareBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_statermentsBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_memoryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_randomPlayBtn_clicked()
{

    int randomPage = QRandomGenerator::global()->bounded(1, 6);
    ui->stackedWidget->setCurrentIndex(randomPage);

    // Запускаем соответствующую игру
    switch(randomPage) {
    case 1: on_sequenceBtn_clicked(); break;
    case 2: on_excessBtn_clicked(); break;
    case 3: on_compareBtn_clicked(); break;
    case 4: on_statermentsBtn_clicked(); break;
    case 5: on_memoryBtn_clicked(); break;
    }
}

void MainWindow::on_BtnSeqBack_clicked()
{
    m_seqTimer->stop();
    returnToMainMenu();
}

void MainWindow::returnToMainMenu()
{
    ui->stackedWidget->setCurrentIndex(0);
    updateAllStatsLabels();
}

void MainWindow::generateSequence()
{
    // Генерация арифметической прогрессии
    int start = QRandomGenerator::global()->bounded(1, 20);

    // Шаг зависит от сложности
    int stepMin = 1;
    int stepMax = 3 + m_difficulty;
    int step = QRandomGenerator::global()->bounded(stepMin, stepMax);

    // Случайно делаем шаг отрицательным (убывающая прогрессия)
    if (QRandomGenerator::global()->bounded(2) == 1) {
        step = -step;
    }

    // Формируем строку последовательности (4 элемента)
    QString sequence;
    for (int i = 0; i < 4; i++) {
        sequence += QString::number(start + i * step);
        if (i < 3) sequence += ", ";
    }

    // Правильный ответ - 5-й элемент
    m_correctAnswer = start + 4 * step;

    // Отображаем
    ui->labelSeqQuestion->setText("Продолжите последовательность:\n" + sequence + ", ?");
    ui->inputSeqAnswer->clear();
    ui->inputSeqAnswer->setFocus();
    ui->labelSeqFeedback->clear();
}

void MainWindow::on_btnSeqSubmit_clicked()
{
    QString inputText = ui->inputSeqAnswer->text();
    bool ok;
    int answer = inputText.toInt(&ok);

    if (!ok) {
        ui->labelSeqFeedback->setText("Введите число");
        return;
    }

    bool correct = (answer == m_correctAnswer);

    if (correct) {
        ui->labelSeqFeedback->setText("Верно +" + QString::number(10 + m_combo * 5));
        addScore(10 + m_combo * 5);
        updateCombo(true);
        m_seqTimer->stop();

        // Следующее задание через 1.5 секунды
        QTimer::singleShot(1500, this, [this]() {
            generateSequence();
            m_timeLeft = 30 - (m_difficulty * 3);
            if (m_timeLeft < 10) m_timeLeft = 10;
            ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLeft) + "с");
            m_seqTimer->start(1000);
        });
    } else {
        ui->labelSeqFeedback->setText("Неверно, правильно: " + QString::number(m_correctAnswer));
        updateCombo(false);
    }
}

void MainWindow::updateSeqTimer()
{
    m_timeLeft--;
    ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLeft) + "с");

    if (m_timeLeft <= 0) {
        m_seqTimer->stop();
        ui->labelSeqFeedback->setText("Время вышло! Ответ: " + QString::number(m_correctAnswer));
        updateCombo(false);

        QTimer::singleShot(2000, this, [this]() {
            generateSequence();
            m_timeLeft = 30 - (m_difficulty * 3);
            if (m_timeLeft < 10) m_timeLeft = 10;
            ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLeft) + "с");
            m_seqTimer->start(1000);
        });
    }
}

// Система очков и статистика

void MainWindow::addScore(int points)
{
    m_score += points;
    updateAllStatsLabels();
    adjustDifficulty();
}

void MainWindow::updateCombo(bool correct)
{
    if (correct) {
        m_combo++;
    } else {
        m_combo = 0;
    }
    updateAllStatsLabels();
}

void MainWindow::adjustDifficulty()
{
    // Сложность растет каждые 50 очков
    int newDifficulty = 1 + (m_score / 50);
    if (newDifficulty > 5) newDifficulty = 5;
    m_difficulty = newDifficulty;
    updateAllStatsLabels();
}

void MainWindow::updateAllStatsLabels()
{
    // Обновляем лейблы на главной странице
    ui->label_score->setText("🏆 Очки: " + QString::number(m_score));
    ui->label_combo->setText("🔥 Комбо: x" + QString::number(m_combo));

    // Отображение сложности
    QString diffText;
    switch(m_difficulty) {
    case 1: diffText = "🟢 Легко"; break;
    case 2: diffText = "🟡 Средне"; break;
    case 3: diffText = "🟠 Сложно"; break;
    case 4: diffText = "🔴 Очень сложно"; break;
    default: diffText = "💀 Эксперт"; break;
    }
    ui->label_difficulty->setText("📊 " + diffText);
}