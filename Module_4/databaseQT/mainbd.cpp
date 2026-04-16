#include "mainbd.h"
#include "ui_mainbd.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("База данных игроков");

    m_playerDb = new PlayerDatabase(this);

    // ... connect'ы ...

    QString dbPath = QDir::currentPath() + "/player_bd.db";
    if (m_playerDb->connect(dbPath)) {
        m_playerDb->createTable();
        m_playerDb->createTop100Table();
        m_playerDb->updateTop100();
        statusBar()->showMessage("База данных подключена", 2000);
    } else {
        statusBar()->showMessage("Ошибка подключения к БД", 2000);
    }

    ui->lineEdit->setPlaceholderText("Логин");
    ui->lineEdit_2->setPlaceholderText("Фамилия");
    ui->lineEdit_3->setPlaceholderText("Имя");

    // ===== НАСТРОЙКА LISTVIEW =====
    ui->listView->setFlow(QListView::TopToBottom);      // Сверху вниз
    ui->listView->setWrapping(false);                   // Без переноса элементов
    ui->listView->setLayoutMode(QListView::SinglePass);
    ui->listView->setViewMode(QListView::ListMode);
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Убираем горизонтальный скролл
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // ===== ВАЖНО: ПЕРЕНОС ТЕКСТА ВНУТРИ ЭЛЕМЕНТА =====
    ui->listView->setWordWrap(true);                    // Перенос текста на новую строку
    ui->listView->setTextElideMode(Qt::ElideNone);      // Не обрезать текст троеточием
    ui->listView->setResizeMode(QListView::Adjust);     // Подстраивать размер

    // ===== МОДЕЛЬ =====
    m_leaderboardModel = new QStandardItemModel(this);
    m_leaderboardModel->setColumnCount(1);
    ui->listView->setModel(m_leaderboardModel);

    updateLeaderboard();

    statusBar()->showMessage("Готов к работе", 2000);
}
MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================
// ОБНОВИТЬ ТАБЛИЦУ ЛИДЕРОВ В LISTVIEW
// ============================================
void MainWindow::updateLeaderboard()
{
    m_leaderboardModel->removeRows(0, m_leaderboardModel->rowCount());

    QList<PlayerData> topPlayers = m_playerDb->getSavedTop100();

    if (topPlayers.isEmpty()) {
        QStandardItem *item = new QStandardItem("🏆 Таблица лидеров пуста");
        item->setTextAlignment(Qt::AlignCenter);
        m_leaderboardModel->appendRow(item);
        return;
    }

    // Заголовок
    QStandardItem *headerItem = new QStandardItem("🏆 ТОП-100 ИГРОКОВ 🏆");
    headerItem->setTextAlignment(Qt::AlignCenter);
    headerItem->setBackground(QBrush(QColor(70, 130, 180)));
    headerItem->setForeground(QBrush(Qt::white));
    headerItem->setSelectable(false);
    m_leaderboardModel->appendRow(headerItem);

    int rank = 1;
    for (const PlayerData &p : topPlayers) {
        QString medal;
        if (rank == 1) medal = "🥇";
        else if (rank == 2) medal = "🥈";
        else if (rank == 3) medal = "🥉";
        else medal = "  ";

        // ===== БОЛЕЕ КОМПАКТНЫЙ ФОРМАТ =====
        QString displayText = QString("%1 %2. %3 %4\n🎯 %5 очков  💎 %6 алмазов  ID: %7")
                                  .arg(medal)
                                  .arg(rank++, 3)
                                  .arg(p.name)
                                  .arg(p.lastName)
                                  .arg(p.points)
                                  .arg(p.diamonds)
                                  .arg(p.id);

        QStandardItem *item = new QStandardItem(displayText);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        // Цвет фона для топ-3
        if (rank - 1 == 1) {
            item->setBackground(QBrush(QColor(255, 215, 0, 40)));   // Золотой
        } else if (rank - 1 == 2) {
            item->setBackground(QBrush(QColor(192, 192, 192, 40))); // Серебряный
        } else if (rank - 1 == 3) {
            item->setBackground(QBrush(QColor(205, 127, 50, 40)));  // Бронзовый
        } else if ((rank - 1) % 2 == 0) {
            item->setBackground(QBrush(QColor(245, 245, 245)));     // Чередование
        }

        item->setSelectable(false);

        // ===== РАЗРЕШАЕМ ПЕРЕНОС ТЕКСТА =====
        item->setTextAlignment(Qt::AlignLeft);

        m_leaderboardModel->appendRow(item);
    }

    // Футер
    QString footerText = QString("━━━━━━━━━━━━━━━━━━━━━━━━\nВсего: %1 игроков").arg(topPlayers.size());
    QStandardItem *footerItem = new QStandardItem(footerText);
    footerItem->setTextAlignment(Qt::AlignCenter);
    footerItem->setBackground(QBrush(QColor(220, 220, 220)));
    footerItem->setSelectable(false);
    m_leaderboardModel->appendRow(footerItem);

    statusBar()->showMessage(QString("✅ Таблица лидеров обновлена: %1 игроков").arg(topPlayers.size()), 3000);
}

// ============================================
// КНОПКА "ВНЕСТИ ДАННЫЕ"
// ============================================
void MainWindow::on_pushButton_clicked()
{
    QString login = ui->lineEdit->text().trimmed();
    QString lastName = ui->lineEdit_2->text().trimmed();
    QString name = ui->lineEdit_3->text().trimmed();

    if (login.isEmpty() || lastName.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Заполните все поля:\nЛогин, Фамилия, Имя");
        return;
    }

    PlayerData newPlayer(name, lastName, login);

    if (m_playerDb->savePlayer(newPlayer)) {
        clearInputs();

        // ===== ОБНОВЛЯЕМ LISTVIEW ПОСЛЕ ДОБАВЛЕНИЯ =====
        updateLeaderboard();

        statusBar()->showMessage(
            QString("✓ Игрок добавлен: %1 %2 (Логин: %3, ID: %4)")
                .arg(name, lastName, login).arg(newPlayer.id),
            5000);

        qDebug() << "✅ Игрок сохранён в БД!";
        qDebug() << "   ID:" << newPlayer.id;
        qDebug() << "   Имя:" << name;
        qDebug() << "   Фамилия:" << lastName;
        qDebug() << "   Логин:" << login;
    }
}

// ============================================
// КНОПКА 2 (тест: начислить очки)
// ============================================
void MainWindow::on_pushButton_2_clicked()
{
    int playerId = 2;
    int pointsToAdd = 500;

    if (m_playerDb->addPoints(playerId, pointsToAdd)) {
        qDebug() << "✅ Игроку ID" << playerId << "начислено" << pointsToAdd << "очков";

        // ===== ОБНОВЛЯЕМ LISTVIEW ПОСЛЕ ИЗМЕНЕНИЯ ОЧКОВ =====
        updateLeaderboard();
    }
}

// ============================================
// КНОПКА "ТОП-100"
// ============================================
void MainWindow::on_top100Button_clicked()
{
    m_playerDb->updateTop100();
    updateLeaderboard();

    qDebug() << "\n╔══════════════════════════════════════════════════════════════╗";
    qDebug() << "║                    🏆 ТОП-100 ИГРОКОВ 🏆                      ║";
    qDebug() << "║              (от большего к меньшему по очкам)                ║";
    qDebug() << "╠══════════════════════════════════════════════════════════════╣";

    QList<PlayerData> topPlayers = m_playerDb->getSavedTop100();
    int rank = 1;
    for (const PlayerData &p : topPlayers) {
        QString medal;
        if (rank == 1) medal = "🥇";
        else if (rank == 2) medal = "🥈";
        else if (rank == 3) medal = "🥉";
        else medal = "  ";

        qDebug() << QString("║ %1 %2. %3 %4 │ 🎯 %5 очков │ 💎 %6 алмазов │ ID: %7")
                        .arg(medal)
                        .arg(rank++, 3)
                        .arg(p.name, -12)
                        .arg(p.lastName, -12)
                        .arg(p.points, 6)
                        .arg(p.diamonds, 4)
                        .arg(p.id, 4);
    }
    qDebug() << "╚══════════════════════════════════════════════════════════════╝\n";
}

// ============================================
// КНОПКА "ПОКАЗАТЬ ТАБЛИЦУ ЛИДЕРОВ"
// ============================================
void MainWindow::on_showLeaderboardButton_clicked()
{
    m_playerDb->updateTop100();
    updateLeaderboard();
    statusBar()->showMessage("Таблица лидеров обновлена", 3000);
}

// ============================================
// КНОПКА "ТОП-20 ПО ОЧКАМ"
// ============================================
void MainWindow::on_top20PointsButton_clicked()
{
    QList<PlayerData> topPlayers = m_playerDb->getTop20ByPoints();

    if (topPlayers.isEmpty()) {
        statusBar()->showMessage("В базе данных нет игроков", 3000);
        return;
    }

    qDebug() << "\n========================================";
    qDebug() << "🏆 ТОП-20 ИГРОКОВ ПО ОЧКАМ 🏆";
    qDebug() << "========================================";

    int rank = 1;
    for (const PlayerData &p : topPlayers) {
        qDebug() << QString("%1. %2 %3 | Очки: %4 | Алмазы: %5 | ID: %6")
                        .arg(rank++, 2)
                        .arg(p.name, -15)
                        .arg(p.lastName, -15)
                        .arg(p.points, 6)
                        .arg(p.diamonds, 4)
                        .arg(p.id, 3);
    }
    qDebug() << "========================================\n";

    PlayerData leader = topPlayers.first();
    statusBar()->showMessage(
        QString("🏆 Лидер: %1 %2 — %3 очков")
            .arg(leader.name, leader.lastName).arg(leader.points),
        5000);
}

// ============================================
// КНОПКА "ТОП-20 ПО АЛМАЗАМ"
// ============================================
void MainWindow::on_top20DiamondsButton_clicked()
{
    QList<PlayerData> topPlayers = m_playerDb->getTop20ByDiamonds();

    if (topPlayers.isEmpty()) {
        statusBar()->showMessage("В базе данных нет игроков", 3000);
        return;
    }

    qDebug() << "\n========================================";
    qDebug() << "💎 ТОП-20 ИГРОКОВ ПО АЛМАЗАМ 💎";
    qDebug() << "========================================";

    int rank = 1;
    for (const PlayerData &p : topPlayers) {
        qDebug() << QString("%1. %2 %3 | Алмазы: %4 | Очки: %5 | ID: %6")
                        .arg(rank++, 2)
                        .arg(p.name, -15)
                        .arg(p.lastName, -15)
                        .arg(p.diamonds, 4)
                        .arg(p.points, 6)
                        .arg(p.id, 3);
    }
    qDebug() << "========================================\n";

    PlayerData leader = topPlayers.first();
    statusBar()->showMessage(
        QString("💎 Лидер по алмазам: %1 %2 — %3 💎")
            .arg(leader.name, leader.lastName).arg(leader.diamonds),
        5000);
}

// ============================================
// ОЧИСТИТЬ ПОЛЯ ВВОДА
// ============================================
void MainWindow::clearInputs()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit->setFocus();
}