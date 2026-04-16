#include "gamewidget.h"
#include "./ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    game = new ColorMemory(this);

    showSequenceTimer = new QTimer(this);
    connect(showSequenceTimer, &QTimer::timeout, this, &GameWidget::showNextColor);

    hideDelayTimer = new QTimer(this);
    hideDelayTimer->setSingleShot(true);
    connect(hideDelayTimer, &QTimer::timeout, this, &GameWidget::hideDisplay);

    connect(game, &ColorMemory::sequenceUpdated, this, &GameWidget::onSequenceUpdated);
    connect(game, &ColorMemory::correctGuess, this, &GameWidget::onCorrectGuess);
    connect(game, &ColorMemory::incorrectGuess, this, &GameWidget::onIncorrectGuess);
    connect(game, &ColorMemory::roundComplete, this, &GameWidget::onRoundComplete);
    connect(game, &ColorMemory::gameOver, this, &GameWidget::onGameOver);

    connect(ui->redButton, &QPushButton::clicked, this, &GameWidget::onRedClick);
    connect(ui->blueButton, &QPushButton::clicked, this, &GameWidget::onBlueClick);
    connect(ui->yellowButton, &QPushButton::clicked, this, &GameWidget::onYellowClick);
    connect(ui->greenButton, &QPushButton::clicked, this, &GameWidget::onGreenClick);
    connect(ui->startButton, &QPushButton::clicked, this, &GameWidget::on_startButton_clicked);

    hideDisplay();
    disableColorButtons(true);
    ui->statusLabel->setText("Нажми START для начала");
    ui->scoreLabel->setText("Счёт: 0");
    ui->sequenceLabel->setText("");
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::setDisplayColor(const QString& color)
{
    QString style;
    if (color == "red") style = "background-color: red; border-radius: 20px;";
    else if (color == "blue") style = "background-color: blue; border-radius: 20px;";
    else if (color == "yellow") style = "background-color: yellow; border-radius: 20px;";
    else if (color == "green") style = "background-color: green; border-radius: 20px;";
    else style = "background-color: #333333; border-radius: 20px; border: 2px solid #555555;";
    ui->colorDisplay->setStyleSheet(style);
}

void GameWidget::hideDisplay()
{
    setDisplayColor("none");
}

void GameWidget::startShowingSequence()
{
    currentShowIndex = 0;
    currentSequence = game->getSequence();
    disableColorButtons(true);
    ui->startButton->setEnabled(false);
    ui->statusLabel->setText("СМОТРИ И ЗАПОМИНАЙ!");
    showSequenceTimer->stop();
    hideDelayTimer->stop();
    hideDisplay();
    showSequenceTimer->setInterval(800);
    if (!currentSequence.isEmpty()) {
        setDisplayColor(currentSequence[0]);
        currentShowIndex++;
        hideDelayTimer->start(100);
    }
    showSequenceTimer->start();
}

void GameWidget::showNextColor()
{
    if (currentShowIndex < currentSequence.size()) {
        setDisplayColor(currentSequence[currentShowIndex]);
        currentShowIndex++;
        hideDelayTimer->start(500);
    } else {
        showSequenceTimer->stop();
        hideDelayTimer->stop();
        hideDisplay();
        ui->statusLabel->setText("ТВОЯ ОЧЕРЕДЬ!");
        disableColorButtons(false);
    }
}

void GameWidget::on_startButton_clicked()
{
    game->reset();
    game->startNewRound();
}

void GameWidget::onRedClick()    { disableColorButtons(true); game->checkColor("red"); }
void GameWidget::onBlueClick()   { disableColorButtons(true); game->checkColor("blue"); }
void GameWidget::onYellowClick() { disableColorButtons(true); game->checkColor("yellow"); }
void GameWidget::onGreenClick()  { disableColorButtons(true); game->checkColor("green"); }

void GameWidget::disableColorButtons(bool disable)
{
    ui->redButton->setEnabled(!disable);
    ui->blueButton->setEnabled(!disable);
    ui->yellowButton->setEnabled(!disable);
    ui->greenButton->setEnabled(!disable);
}

void GameWidget::onSequenceUpdated(const QVector<QString>& sequence)
{
    QString text;
    for (const QString& c : sequence) text += c + " ";
    ui->sequenceLabel->setText(text.trimmed());
    ui->scoreLabel->setText(QString("Счёт: %1").arg(game->getScore()));
    startShowingSequence();
}

void GameWidget::onCorrectGuess()
{
    ui->statusLabel->setText("✓ Правильно!");
    disableColorButtons(false);
    hideDisplay();
}

void GameWidget::onIncorrectGuess()
{
    ui->statusLabel->setText("✗ Неправильно!");
    disableColorButtons(true);
    ui->startButton->setEnabled(true);
    hideDisplay();
}

void GameWidget::onRoundComplete()
{
    ui->statusLabel->setText("★ РАУНД ПРОЙДЕН! ★");
    disableColorButtons(true);
    showSequenceTimer->stop();
    hideDelayTimer->stop();
    hideDisplay();
    QTimer::singleShot(2000, this, [this]() { game->startNewRound(); });
}

void GameWidget::onGameOver()
{
    showSequenceTimer->stop();
    hideDelayTimer->stop();
    hideDisplay();
    disableColorButtons(true);
    ui->startButton->setEnabled(true);
    ui->statusLabel->setText("GAME OVER");
    int finalScore = game->getScore();
    emit gameFinished(finalScore);
    QMessageBox::information(this, "Игра окончена", QString("Вы проиграли!\nВаш счёт: %1").arg(finalScore));
}
