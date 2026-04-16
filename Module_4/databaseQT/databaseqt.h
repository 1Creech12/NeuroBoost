#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>

// ============================================
// КЛАСС ДАННЫХ ИГРОКА
// ============================================
class PlayerData
{
public:
    int id = -1;
    QString name;
    QString lastName;
    QString login;
    int points = 0;
    int diamonds = 0;

    PlayerData() = default;
    PlayerData(const QString &name, const QString &lastName, const QString &login)
        : name(name), lastName(lastName), login(login) {}

    PlayerData(const QString &name, const QString &lastName, const QString &login, int points, int diamonds)
        : name(name), lastName(lastName), login(login), points(points), diamonds(diamonds) {}

    void setName(const QString &newName) { name = newName; }
    void setLastName(const QString &newLastName) { lastName = newLastName; }
    void setLogin(const QString &newLogin) { login = newLogin; }
    void setPoints(int newPoints) { points = newPoints; }
    void setDiamonds(int newDiamonds) { diamonds = newDiamonds; }

    QString getName() const { return name; }
    QString getLastName() const { return lastName; }
    QString getLogin() const { return login; }
    int getPoints() const { return points; }
    int getDiamonds() const { return diamonds; }
};

// ============================================
// КЛАСС РАБОТЫ С БАЗОЙ ДАННЫХ ИГРОКОВ
// ============================================
class PlayerDatabase : public QObject
{
    Q_OBJECT

public:
    explicit PlayerDatabase(QObject *parent = nullptr);
    ~PlayerDatabase();

    // Подключение к БД
    bool connect(const QString &dbPath = "player_bd.db");
    void disconnect();
    bool isConnected() const;

    // Создание таблицы
    bool createTable();

    // Сохранение игрока
    bool savePlayer(PlayerData &player);

    // Удаление игрока по ID
    bool deletePlayer(int userId);

    // Поиск игрока по логину
    bool playerExists(const QString &login);

    // Получение игрока по ID
    PlayerData getPlayerById(int userId);

    // Получение игрока по логину
    PlayerData getPlayerByLogin(const QString &login);

    // Получение всех игроков
    QList<PlayerData> getAllPlayers(bool sortByPointsDesc = false);

    // Получение модели для QTableView
    QSqlTableModel* getTableModel();

    // Получение текста последней ошибки
    QString lastError() const;

    // Добавить очки игроку по ID
    bool addPoints(int userId, int pointsToAdd);

    // Добавить алмазы игроку по ID
    bool addDiamonds(int userId, int diamondsToAdd);

    // Установить точное количество очков
    bool setPoints(int userId, int newPoints);

    // Установить точное количество алмазов
    bool setDiamonds(int userId, int newDiamonds);

    // Получить текущие очки игрока
    int getPlayerPoints(int userId);

    // Получить текущие алмазы игрока
    int getPlayerDiamonds(int userId);

    // ===== ТОПЫ И РЕЙТИНГИ =====

    // Топ-20 по очкам
    QList<PlayerData> getTop20ByPoints();
    QList<PlayerData> getTopPlayersByPoints(int limit = 20);

    // Топ-20 по алмазам
    QList<PlayerData> getTop20ByDiamonds();
    QList<PlayerData> getTopPlayersByDiamonds(int limit = 20);

    // ===== ТОП-100 ТАБЛИЦА (СОХРАНЯЕМАЯ) =====

    // Создать таблицу для хранения топ-100
    bool createTop100Table();

    // Обновить топ-100 (очистить старый и записать новый)
    bool updateTop100();

    // Получить топ-100 из сохранённой таблицы
    QList<PlayerData> getSavedTop100();

    // Очистить таблицу топ-100
    bool clearTop100();

signals:
    void connected();
    void disconnected();
    void playerSaved(int playerId);
    void playerDeleted(int playerId);
    void errorOccurred(const QString &message);
    void infoMessage(const QString &message);

private:
    QSqlDatabase m_db;
    QString m_connectionName;

    bool execQuery(QSqlQuery &query, const QString &errorPrefix = "");
};

#endif // PLAYERDATABASE_H