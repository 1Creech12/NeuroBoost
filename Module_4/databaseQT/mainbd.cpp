#include "mainbd.h"
#include "ui_mainbd.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QDir>        // ← ДОБАВИТЬ!
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("База данных игроков");

    // Создаём объект БД
    m_playerDb = new PlayerDatabase(this);

    // Подключаем сигналы
    connect(m_playerDb, &PlayerDatabase::errorOccurred,
            this, [this](const QString &err) {
                QMessageBox::critical(this, "Ошибка БД", err);
                statusBar()->showMessage("Ошибка: " + err, 5000);
            });

    connect(m_playerDb, &PlayerDatabase::infoMessage,
            this, [this](const QString &msg) {
                statusBar()->showMessage(msg, 3000);
            });

    // Подключаемся к БД
    QString dbPath = QDir::currentPath() + "/player_bd.db";

    if (m_playerDb->connect(dbPath)) {
        m_playerDb->createTable();
        statusBar()->showMessage("База данных подключена", 2000);
    } else {
        statusBar()->showMessage("Ошибка подключения к БД", 2000);
    }

    // Настраиваем таблицу
    ui->tableView->setModel(m_playerDb->getTableModel());
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Подсказки в полях ввода
    ui->lineEdit->setPlaceholderText("Логин");
    ui->lineEdit_2->setPlaceholderText("Фамилия");
    ui->lineEdit_3->setPlaceholderText("Имя");

    statusBar()->showMessage("Готов к работе", 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// ============================================
// КНОПКА "ВНЕСТИ ДАННЫЕ"
// ============================================
void MainWindow::on_pushButton_clicked()
{
    // 1. Получаем текст из полей ввода
    //    lineEdit     → Логин
    //    lineEdit_2   → Фамилия
    //    lineEdit_3   → Имя
    QString login = ui->lineEdit->text().trimmed();
    QString lastName = ui->lineEdit_2->text().trimmed();
    QString name = ui->lineEdit_3->text().trimmed();

    // 2. Проверяем, что все поля заполнены
    if (login.isEmpty() || lastName.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Заполните все поля:\nЛогин, Фамилия, Имя");
        return;
    }

    // 3. Создаём объект игрока (Имя, Фамилия, Логин)
    PlayerData newPlayer(name, lastName, login);

    // 4. Сохраняем в БД
    if (m_playerDb->savePlayer(newPlayer)) {
        // 5. Очищаем поля
        clearInputs();

        // 6. Обновляем таблицу
        refreshTable();

        // 7. Сообщение об успехе
        statusBar()->showMessage(
            QString("✓ Игрок добавлен: %1 %2 (Логин: %3)")
                .arg(name, lastName, login),
            5000);
    }
}

// ============================================
// ОЧИСТИТЬ ПОЛЯ ВВОДА
// ============================================
void MainWindow::clearInputs()
{
    ui->lineEdit->clear();      // Логин
    ui->lineEdit_2->clear();    // Фамилия
    ui->lineEdit_3->clear();    // Имя
    ui->lineEdit->setFocus();   // Курсор в поле Логин
}

// ============================================
// ОБНОВИТЬ ТАБЛИЦУ
// ============================================
void MainWindow::refreshTable()
{
    if (auto model = qobject_cast<QSqlTableModel*>(ui->tableView->model())) {
        model->select();  // Перезагружаем данные из БД
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    m_playerDb->addPoints(2,500);
    refreshTable();
}

