#include "gamewidget.h"
#include "./ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent, PlayerDatabase* db, PlayerData* player)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
    , m_db(db)
    , m_currentPlayer(player)
{
    ui->setupUi(this);

    this->setFixedSize(700, 600);

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
    connect(game, &ColorMemory::levelUp, this, &GameWidget::onLevelUp);

    connect(ui->redButton, &QPushButton::clicked, this, &GameWidget::onRedClick);
    connect(ui->blueButton, &QPushButton::clicked, this, &GameWidget::onBlueClick);
    connect(ui->yellowButton, &QPushButton::clicked, this, &GameWidget::onYellowClick);
    connect(ui->greenButton, &QPushButton::clicked, this, &GameWidget::onGreenClick);
    connect(ui->startButton, &QPushButton::clicked, this, &GameWidget::on_startButton_clicked);

    hideDisplay();
    disableColorButtons(true);
    ui->statusLabel->setText("Нажми START для начала");
    ui->scoreLabel->setText("Счёт: 0");
    ui->levelLabel->setText("Уровень: 1");
}

GameWidget::~GameWidget()
{
    delete ui;
}

int GameWidget::getShowInterval() const {
    int level = game->getLevel();

    if (level >= 6) return 350;
    return 800 - (level - 1) * 100;
}

int GameWidget::getHideInterval() const {
    int level = game->getLevel();
    if (level >= 6) return 150;
    return 400 - (level - 1) * 60;
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

    showSequenceTimer->setInterval(getShowInterval());

    if (!currentSequence.isEmpty()) {
        setDisplayColor(currentSequence[0]);
        currentShowIndex++;
        hideDelayTimer->start(getHideInterval());
    }

    showSequenceTimer->start();
}

void GameWidget::showNextColor()
{
    if (currentShowIndex < currentSequence.size()) {
        setDisplayColor(currentSequence[currentShowIndex]);
        currentShowIndex++;
        hideDelayTimer->start(getHideInterval());
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
    ui->levelLabel->setText("Уровень: 1");
    ui->scoreLabel->setText("Счёт: 0");
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
    ui->scoreLabel->setText(QString("Счёт: %1").arg(game->getScore()));
    ui->levelLabel->setText(QString("Уровень: %1").arg(game->getLevel()));
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
    int level = game->getLevel();
    QString levelText = (level > 5) ? "БЕСКОНЕЧНЫЙ" : QString::number(level);
    ui->statusLabel->setText(QString("★ РАУНД ПРОЙДЕН! (Уровень %1) ★").arg(levelText));
    disableColorButtons(true);
    showSequenceTimer->stop();
    hideDelayTimer->stop();
    hideDisplay();

    QTimer::singleShot(1500, this, [this]() {
        game->startNewRound();
    });
}

void GameWidget::onLevelUp(int newLevel) {
    QString levelText = (newLevel > 5) ? "БЕСКОНЕЧНЫЙ" : QString::number(newLevel);
    ui->levelLabel->setText(QString("Уровень: %1").arg(levelText));

    QString msg;
    if (newLevel == 6) {
        msg = "🏆 БЕСКОНЕЧНЫЙ РЕЖИМ! 🏆\nТеперь игра на очки!";
    } else {
        msg = QString("🎉 УРОВЕНЬ %1! 🎉\nБольше цветов, выше скорость!").arg(newLevel);
    }

    QMessageBox::information(this, "Повышение уровня", msg);
}

void GameWidget::onGameOver()
{
    showSequenceTimer->stop();
    hideDelayTimer->stop();
    hideDisplay();
    disableColorButtons(true);
    ui->startButton->setEnabled(true);

    int finalScore = game->getScore();
    int finalLevel = game->getLevel();
    QString levelText = (finalLevel > 5) ? "БЕСКОНЕЧНЫЙ" : QString::number(finalLevel);

    ui->statusLabel->setText("GAME OVER");

    if (m_db && m_currentPlayer) {
        int currentBest = m_currentPlayer->getPoints();
        if (finalScore > currentBest) {
            m_currentPlayer->setPoints(finalScore);
            if (m_db->savePlayer(*m_currentPlayer)) {
                QMessageBox::information(this, "Рекорд!",
                                         QString("🎉 Новый рекорд: %1 очков! 🎉").arg(finalScore));
            }
        }
    }

    emit gameFinished(finalScore);
    QMessageBox::information(this, "Игра окончена",
                             QString("Вы проиграли!\n\n"
                                     "Уровень: %1\n"
                                     "Ваш счёт: %2").arg(levelText).arg(finalScore));
}