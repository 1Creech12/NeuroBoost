#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isProcessing(false), currentA(0), currentB(0), currentOp('+')
    , difficulty(1), problemsSolved(0)
    , isComplexMode(false), gameEnded(false)
{
    ui->setupUi(this);
    ui->lineEdit->hide();
    ui->label_2->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Вычисление с приоритетом: сначала * и /, потом + и -
int MainWindow::evaluateComplex()
{
    int n[4] = {nums[0], nums[1], nums[2], nums[3]};
    char o[3] = {ops[0], ops[1], ops[2]};
    int count = 3;

    for (int i = 0; i < count; ) {
        if (o[i] == '*' || o[i] == '/') {
            if (o[i] == '*') n[i] = n[i] * n[i+1];
            else n[i] = (n[i+1] != 0) ? n[i] / n[i+1] : 0;

            for (int j = i; j < count - 1; ++j) {
                n[j+1] = n[j+2];
                o[j] = o[j+1];
            }
            count--;
        } else {
            i++;
        }
    }

    int result = n[0];
    for (int i = 0; i < count; ++i) {
        if (o[i] == '+') result += n[i+1];
        else if (o[i] == '-') result -= n[i+1];
    }
    return result;
}

void MainWindow::generateProblem()
{
    if (gameEnded) {
        ui->label->setText("🎮 Конец игры");
        ui->lineEdit->hide();
        return;
    }

    isComplexMode = (problemsSolved >= 7 && problemsSolved < 10);
    int maxRange = 15 * difficulty;

    if (isComplexMode) {
        for (int i = 0; i < 4; ++i) nums[i] = QRandomGenerator::global()->bounded(1, maxRange + 1);
        for (int i = 0; i < 3; ++i) {
            ops[i] = "+-*/"[QRandomGenerator::global()->bounded(4)];
            if (ops[i] == '/' && nums[i+1] == 0) nums[i+1] = 1;
        }
        ui->label->setText(QString("%1 %2 %3 %4 %5 %6 %7 = ?")
                               .arg(nums[0]).arg(ops[0]).arg(nums[1])
                               .arg(ops[1]).arg(nums[2]).arg(ops[2]).arg(nums[3]));
    } else {
        currentA = QRandomGenerator::global()->bounded(1, maxRange + 1);
        currentB = QRandomGenerator::global()->bounded(1, maxRange + 1);
        currentOp = "+-*/"[QRandomGenerator::global()->bounded(4)];
        if (currentOp == '/' && currentB == 0) currentB = 1;
        ui->label->setText(QString("%1 %2 %3 = ?").arg(currentA).arg(currentOp).arg(currentB));
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (gameEnded) return;
    ui->pushButton->hide();
    ui->lineEdit->show();
    ui->lineEdit->clear();
    ui->lineEdit->setReadOnly(false);
    ui->lineEdit->setFocus();

    problemsSolved = 0;
    difficulty = 1;
    gameEnded = false;
    ui->label_2->clear();
    generateProblem();
}

void MainWindow::on_lineEdit_returnPressed()
{
    // 🔒 Блокируем повторные нажатия
    if (isProcessing || gameEnded) return;
    isProcessing = true;

    QString inputText = ui->lineEdit->text().trimmed();
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите число!");
        isProcessing = false;
        return;
    }

    bool ok;
    int userAnswer = inputText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное целое число!");
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        isProcessing = false;
        return;
    }

    // Вычисляем правильный ответ
    int correctAnswer = isComplexMode ? evaluateComplex() : 0;
    if (!isComplexMode) {
        switch (currentOp) {
        case '+': correctAnswer = currentA + currentB; break;
        case '-': correctAnswer = currentA - currentB; break;
        case '*': correctAnswer = currentA * currentB; break;
        case '/': correctAnswer = currentA / currentB; break;
        }
    }

    // 🟢 Выводим результат в label_2 (он НЕ очищается автоматически!)
    if (userAnswer == correctAnswer) {
        ui->label_2->setText("✅ Правильно!");
    } else {
        ui->label_2->setText(QString("❌ Неправильно! Ответ: %1").arg(correctAnswer));
    }

    problemsSolved++;
    qDebug() << "[DEBUG] Решено задач:" << problemsSolved;

    // 🛑 Конец игры после 10 задач
    if (problemsSolved >= 10) {
        gameEnded = true;
        ui->label->setText("🎮 Конец игры");
        ui->lineEdit->hide();
        ui->label_2->setText("🏆 Поздравляем! Игра пройдена!");
        QMessageBox::information(this, "Финал", "Все 10 задач решены! 🎉");
        isProcessing = false;
        return;
    }

    // ⏳ Быстрый переход к следующему примеру (100 мс)
    // label_2 НЕ очищаем! Надпись останется до следующего ответа.
    ui->lineEdit->setReadOnly(true); // Кратковременная блокировка ввода
    QTimer::singleShot(100, this, [this]() {
        generateProblem();
        ui->lineEdit->clear();
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit->setFocus();
        isProcessing = false; // Разрешаем новый ввод
    });
}