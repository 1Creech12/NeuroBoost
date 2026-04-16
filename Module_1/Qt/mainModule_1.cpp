#include "mainModule_1.h"
#include "ui_mainModule_1.h"
#include "databaseqt.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <climits>

// ============================================
// КОНСТРУКТОР / ДЕСТРУКТОР
// ============================================
MainModule_1::MainModule_1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainModule_1)
    , m_playerDb(new PlayerDatabase(this))
    , m_playerId(1)
    , isProcessing(false)
    , currentA(0), currentB(0), currentOp('+')
    , difficulty(1), problemsSolved(0), correctAnswers(0), currentScore(0)
    , isComplexMode(false), gameEnded(false)
    , totalTimeMs(0), fastestTimeMs(LLONG_MAX), slowestTimeMs(0)
{
    ui->setupUi(this);
    ui->lineEdit->hide();
    ui->label_2->clear();

    // 🔌 Подключение к БД
    if (m_playerDb->connect("player_bd.db")) {
        m_playerDb->createTable();

        // Создаём тестового игрока, если нет
        if (!m_playerDb->playerExists("player1")) {
            PlayerData newPlayer("Игрок", "Тестовый", "player1");
            newPlayer.setPoints(0);
            newPlayer.setDiamonds(0);
            m_playerDb->savePlayer(newPlayer);
            m_playerId = newPlayer.id;
            qDebug() << "[MainModule_1] Создан игрок ID:" << m_playerId;
        }

        loadPlayerData();
        qDebug() << "[MainModule_1] База данных готова";
    } else {
        qDebug() << "[MainModule_1] Ошибка БД:" << m_playerDb->lastError();
    }

    // 📡 Сигналы для отладки
    connect(m_playerDb, &PlayerDatabase::errorOccurred,
            this, [](const QString &err){ qDebug() << "DB Error:" << err; });
    connect(m_playerDb, &PlayerDatabase::infoMessage,
            this, [](const QString &msg){ qDebug() << "DB Info:" << msg; });
}

MainModule_1::~MainModule_1()
{
    delete ui;
}

// ============================================
// ВЫЧИСЛЕНИЕ СЛОЖНОГО ВЫРАЖЕНИЯ
// ============================================
int MainModule_1::evaluateComplex()
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

// ============================================
// ГЕНЕРАЦИЯ ЗАДАЧИ
// ============================================
void MainModule_1::generateProblem()
{
    if (gameEnded) {
        ui->label->setText("🎮 Конец игры");
        ui->lineEdit->hide();
        return;
    }

    answerTimer.restart();
    isComplexMode = (problemsSolved >= 7 && problemsSolved < 10);
    int maxRange = 15 * difficulty;

    if (isComplexMode) {
        for (int i = 0; i < 4; ++i)
            nums[i] = QRandomGenerator::global()->bounded(1, maxRange + 1);
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

// ============================================
// КНОПКА СТАРТ
// ============================================
void MainModule_1::on_pushButton_clicked()
{
    if (gameEnded) return;

    ui->pushButton->hide();
    ui->lineEdit->show();
    ui->lineEdit->clear();
    ui->lineEdit->setReadOnly(false);
    ui->lineEdit->setFocus();

    problemsSolved = 0;
    correctAnswers = 0;
    currentScore = 0;
    difficulty = 1;
    gameEnded = false;
    totalTimeMs = 0;
    fastestTimeMs = LLONG_MAX;
    slowestTimeMs = 0;

    ui->label_2->clear();
    generateProblem();
}

// ============================================
// ОБРАБОТКА ОТВЕТА (Enter)
// ============================================
void MainModule_1::on_lineEdit_returnPressed()
{
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

    qint64 timeMs = answerTimer.elapsed();
    double timeSec = timeMs / 1000.0;

    int correctAnswer = isComplexMode ? evaluateComplex() : 0;
    if (!isComplexMode) {
        switch (currentOp) {
        case '+': correctAnswer = currentA + currentB; break;
        case '-': correctAnswer = currentA - currentB; break;
        case '*': correctAnswer = currentA * currentB; break;
        case '/': correctAnswer = currentA / currentB; break;
        }
    }

    problemsSolved++;
    totalTimeMs += timeMs;
    if (timeMs < fastestTimeMs) fastestTimeMs = timeMs;
    if (timeMs > slowestTimeMs) slowestTimeMs = timeMs;

    QString timeStr = QString::number(timeSec, 'f', 3);

    if (userAnswer == correctAnswer) {
        currentScore += isComplexMode ? 20 : 10;
        correctAnswers++;
        ui->label_2->setText(QString("✅ Правильно! ⏱ %1 с.").arg(timeStr));
    } else {
        ui->label_2->setText(QString("❌ Неправильно! ⏱ %1 с.").arg(timeStr));
    }

    if (problemsSolved >= 10) {
        gameEnded = true;
        ui->label->setText("🎮 Конец игры");
        ui->lineEdit->hide();
        showFinalResults();
        isProcessing = false;
        return;
    }

    ui->lineEdit->setReadOnly(true);
    QTimer::singleShot(100, this, [this]() {
        generateProblem();
        ui->lineEdit->clear();
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit->setFocus();
        isProcessing = false;
    });
}

// ============================================
// 🗄️ ЗАГРУЗКА ДАННЫХ ИГРОКА
// ============================================
void MainModule_1::loadPlayerData()
{
    if (!m_playerDb || !m_playerDb->isConnected()) {
        qDebug() << "[MainModule_1] БД не готова";
        return;
    }

    PlayerData player = m_playerDb->getPlayerByLogin("player1");
    if (player.id > 0) {
        m_playerId = player.id;
        currentScore = player.getPoints();
        qDebug() << "[MainModule_1] Загружено: очки =" << currentScore;
    }
}

// ============================================
// 🗄️ ОБНОВЛЕНИЕ ТАБЛИЦЫ (реализация)
// ============================================
void MainModule_1::refreshTable()
{
    if (m_playerDb && m_playerDb->isConnected()) {
        // Пример: если есть QTableView в UI
        // ui->tableView->setModel(m_playerDb->getTableModel());
        qDebug() << "[MainModule_1] Таблица обновлена";
    }
}

// ============================================
// 🗄️ ОЧИСТКА ПОЛЕЙ ВВОДА (реализация)
// ============================================
void MainModule_1::clearInputs()
{
    if (ui) {
        ui->lineEdit->clear();
        ui->label_2->clear();
    }
    qDebug() << "[MainModule_1] Поля ввода очищены";
}

// ============================================
// ФИНАЛЬНЫЕ РЕЗУЛЬТАТЫ + СОХРАНЕНИЕ ОЧКОВ
// ============================================
void MainModule_1::showFinalResults()
{
    QString totalStr = QString::number(totalTimeMs / 1000.0, 'f', 3);
    QString fastStr = QString::number((fastestTimeMs == LLONG_MAX ? 0 : fastestTimeMs) / 1000.0, 'f', 3);
    QString slowStr = QString::number(slowestTimeMs / 1000.0, 'f', 3);

    QString stats = QString("🏆 Игра окончена!\n"
                            "💰 Итого очков: %1\n"
                            "✅ Правильных ответов: %2 из %3\n"
                            "⏱ Всего времени: %4 с.\n"
                            "⚡ Самый быстрый: %5 с.\n"
                            "🐢 Самый долгий: %6 с.")
                        .arg(currentScore)
                        .arg(correctAnswers)
                        .arg(problemsSolved)
                        .arg(totalStr)
                        .arg(fastStr)
                        .arg(slowStr);

    ui->label_2->setText(stats.replace("\n", "<br>"));

    // 💾 Сохраняем очки в БД
    if (m_playerDb && m_playerDb->isConnected()) {
        m_playerDb->addPoints(m_playerId, currentScore);
        qDebug() << "[MainModule_1] Очки сохранены:" << currentScore;
    }

    QMessageBox::information(this, "Результаты", stats);
}