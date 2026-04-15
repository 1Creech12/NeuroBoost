#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QRandomGenerator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), currentA(0), currentB(0), currentOp('+'), difficulty(1)
{
    ui->setupUi(this);

    ui->pushButtonNext->hide();
    ui->lineEdit->hide();

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_lineEdit_returnPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generateProblem()
{
    int maxRange = 15 * difficulty;
    currentA = QRandomGenerator::global()->bounded(1, maxRange + 1);
    currentB = QRandomGenerator::global()->bounded(1, maxRange + 1);

    int opIndex = QRandomGenerator::global()->bounded(4);
    switch (opIndex) {
    case 0: currentOp = '+'; break;
    case 1: currentOp = '-'; break;
    case 2: currentOp = '*'; break;
    case 3:
        currentOp = '/';
        if (currentB == 0) currentB = 1; // Защита от деления на 0
        break;
    }

    ui->label->setText(QString("%1 %2 %3 = ?").arg(currentA).arg(currentOp).arg(currentB));
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();      // Скрываем кнопку старта
    ui->lineEdit->show();        // Показываем поле ввода
    ui->lineEdit->clear();
    ui->lineEdit->setReadOnly(false);
    ui->lineEdit->setFocus();    // Ставим курсор в поле ввода

    generateProblem();           // Генерируем первый пример
}

void MainWindow::on_lineEdit_returnPressed()
{

    QString inputText = ui->lineEdit->text().trimmed();
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите число!");
        return;
    }

    bool ok;
    int userAnswer = inputText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное целое число!");
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        return;
    }

    // Вычисляем правильный ответ
    int correctAnswer = 0;
    switch (currentOp) {
    case '+': correctAnswer = currentA + currentB; break;
    case '-': correctAnswer = currentA - currentB; break;
    case '*': correctAnswer = currentA * currentB; break;
    case '/': correctAnswer = currentA / currentB; break; // Целочисленное деление///////////////
    }

    // Обновляем Label в зависимости от результата
    if (userAnswer == correctAnswer) {
        ui->label->setText("✅ Правильно!");
    } else {
        ui->label->setText(QString("❌ Неправильно! Ответ: %1").arg(correctAnswer));
    }

    // Показываем скрытую кнопку "Далее" и блокируем ввод до следующего шага
    ui->pushButtonNext->show();
    ui->lineEdit->setReadOnly(true);
}

void MainWindow::on_pushButtonNext_clicked()
{
    difficulty++;
    generateProblem();

    ui->lineEdit->clear();
    ui->lineEdit->setReadOnly(false);
    ui->lineEdit->setFocus();

    ui->pushButtonNext->hide();  // Скрываем кнопку "Далее" до следующего отве
    if (difficulty==3){
        qDebug()<<"следущий уровень";
    }
    else if (difficulty==10){
        qDebug()<<"Стоп игра";
    }
}