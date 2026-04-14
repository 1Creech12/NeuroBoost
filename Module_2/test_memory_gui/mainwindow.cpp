#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    game = new ColorMemory(this);

    // Подключение сигналов
    connect(game, &ColorMemory::updateColors, this, &MainWindow::onUpdateSequence);
    connect(game, &ColorMemory::correctGuess, this, &MainWindow::onCorrectGuess);
    connect(game, &ColorMemory::uncorrectGuess, this, &MainWindow::onUnCorrectGuess);
    connect(game, &ColorMemory::roundComplete, this, &MainWindow::onRoundComplete);
    connect(game, &ColorMemory::gameOver, this, &MainWindow::onGameOver);

    // Подключение кнопок
    connect(ui->redButton, &QPushButton::clicked, this, &MainWindow::on_redButton_clicked);
    connect(ui->blueButton, &QPushButton::clicked, this, &MainWindow::on_blueButton_clicked);
    connect(ui->yellowButton, &QPushButton::clicked, this, &MainWindow::on_yellowButton_clicked);
    connect(ui->greenButton, &QPushButton::clicked, this, &MainWindow::on_greenButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);

    disableColorButtons(true); // кнопки неактивны изначально
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_redButton_clicked()
{
    game->checkColor("red");
}


void MainWindow::on_blueButton_clicked()
{
    game->checkColor("blue");
}


void MainWindow::on_yellowButton_clicked()
{
    game->checkColor("yellow");
}


void MainWindow::on_greenButton_clicked()
{
    game->checkColor("green");
}


void MainWindow::on_startButton_clicked()
{
    game->reset();
    game->startNewRound();
    disableColorButtons(false);
    ui->startButton->setEnabled(false);
}

void MainWindow::onUpdateSequence(const QString& text) {
    ui->colorsText->setText(text);
}

void MainWindow::onCorrectGuess() {
    ui->statusText->setText("Правильно!");
}

void MainWindow::onUnCorrectGuess() {
    ui->statusText->setText("Не правильно!");
}

void MainWindow::onRoundComplete() {
    ui->statusText->setText("Раунд пройден!");
    disableColorButtons(true);

    // Задержка перед следующим раундом
    QTimer::singleShot(1500, this, &MainWindow::nextRound);
}

void MainWindow::onGameOver() {
    QMessageBox::information(this, "Игра окончена",
                             QString("Вы проиграли!\nВаш счет: %1"));
    ui->startButton->setEnabled(true);
}

void MainWindow::nextRound() {
    game->startNewRound();
    disableColorButtons(false);
}

void MainWindow::disableColorButtons(bool disable) {
    ui->redButton->setEnabled(!disable);
    ui->blueButton->setEnabled(!disable);
    ui->yellowButton->setEnabled(!disable);
    ui->greenButton->setEnabled(!disable);
}