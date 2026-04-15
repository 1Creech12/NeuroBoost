#include "mainwindow.h"
#include "ui_mainwindow.h"

MainGames::MainGames(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    game = new ColorMemory(this);

    // Подключение сигналов
    connect(game, &ColorMemory::updateColors, this, &MainGames::onUpdateSequence);
    connect(game, &ColorMemory::correctGuess, this, &MainGames::onCorrectGuess);
    connect(game, &ColorMemory::uncorrectGuess, this, &MainGames::onUnCorrectGuess);
    connect(game, &ColorMemory::roundComplete, this, &MainGames::onRoundComplete);
    connect(game, &ColorMemory::gameOver, this, &MainGames::onGameOver);

    // Подключение кнопок
    connect(ui->redButton, &QPushButton::clicked, this, &MainGames::on_redButton_clicked);
    connect(ui->blueButton, &QPushButton::clicked, this, &MainGames::on_blueButton_clicked);
    connect(ui->yellowButton, &QPushButton::clicked, this, &MainGames::on_yellowButton_clicked);
    connect(ui->greenButton, &QPushButton::clicked, this, &MainGames::on_greenButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainGames::on_startButton_clicked);

    disableColorButtons(true); // кнопки неактивны изначально
}

MainGames::~MainGames() {
    delete ui;
}

void MainGames::on_redButton_clicked()
{
    game->checkColor("red");
}


void MainGames::on_blueButton_clicked()
{
    game->checkColor("blue");
}


void MainGames::on_yellowButton_clicked()
{
    game->checkColor("yellow");
}


void MainGames::on_greenButton_clicked()
{
    game->checkColor("green");
}


void MainGames::on_startButton_clicked()
{
    game->reset();
    game->startNewRound();
    disableColorButtons(false);
    ui->startButton->setEnabled(false);
}

void MainGames::onUpdateSequence(const QString& text) {
    ui->colorsText->setText(text);
}

void MainGames::onCorrectGuess() {
    ui->statusText->setText("Правильно!");
}

void MainGames::onUnCorrectGuess() {
    ui->statusText->setText("Не правильно!");
}

void MainGames::onRoundComplete() {
    ui->statusText->setText("Раунд пройден!");
    disableColorButtons(true);

    // Задержка перед следующим раундом
    QTimer::singleShot(1500, this, &MainGames::nextRound);
}

void MainGames::onGameOver() {
    QMessageBox::information(this, "Игра окончена",
                             QString("Вы проиграли!\nВаш счет: %1"));
    ui->startButton->setEnabled(true);
}

void MainGames::nextRound() {
    game->startNewRound();
    disableColorButtons(false);
}

void MainGames::disableColorButtons(bool disable) {
    ui->redButton->setEnabled(!disable);
    ui->blueButton->setEnabled(!disable);
    ui->yellowButton->setEnabled(!disable);
    ui->greenButton->setEnabled(!disable);
}