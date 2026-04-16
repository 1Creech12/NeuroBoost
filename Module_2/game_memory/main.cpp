#include "gamewidget.h"
#include "databaseqt.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаём и подключаем базу данных
    PlayerDatabase db;
    QString dbPath = QCoreApplication::applicationDirPath() + "/player_bd.db";

    if (!db.connect(dbPath)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось подключиться к базе данных!");
        return -1;
    }

    // Загружаем текущего игрока (например, по логину "default")
    PlayerData currentPlayer = db.getPlayerByLogin("default");

    // Если игрока нет — создаём нового
    if (currentPlayer.id <= 0) {
        currentPlayer = PlayerData("Игрок", "Default", "default");
        currentPlayer.setPoints(0);
        currentPlayer.setDiamonds(0);
        if (!db.savePlayer(currentPlayer)) {
            QMessageBox::warning(nullptr, "Предупреждение", "Не удалось создать игрока по умолчанию");
        }
    }

    // Создаём главное окно игры с БД и игроком
    GameWidget w(nullptr, &db, &currentPlayer);
    w.show();

    return QCoreApplication::exec();
}