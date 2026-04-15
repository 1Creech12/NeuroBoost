#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>

// ============================================
// КЛАСС ДАННЫХ ИГРОКА (аналог вашего Data_Player)
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

    // Конструкторы (как у вас в Data_Player)
    PlayerData() = default;
    PlayerData(const QString &name, const QString &lastName, const QString &login)
        : name(name), lastName(lastName), login(login) {}

    PlayerData(const QString &name, const QString &lastName, const QString &login, int points, int diamonds)
        : name(name), lastName(lastName), login(login), points(points), diamonds(diamonds) {}

    // Методы для установки полей (как у вас setXXX)
    void setName(const QString &newName) { name = newName; }
    void setLastName(const QString &newLastName) { lastName = newLastName; }
    void setLogin(const QString &newLogin) { login = newLogin; }
    void setPoints(int newPoints) { points = newPoints; }
    void setDiamonds(int newDiamonds) { diamonds = newDiamonds; }

    // Методы получения полей
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

    // Создание таблицы (аналог CREATE TABLE в вашем коде)
    bool createTable();

    // ===== ОСНОВНЫЕ МЕТОДЫ (аналоги ваших) =====

    // Сохранение игрока (объединяет INSERT и UPDATE как у вас в saveToBD)
    bool savePlayer(PlayerData &player);

    // Удаление игрока по ID (аналог deleteById)
    bool deletePlayer(int userId);

    // Поиск игрока по логину (для проверки существования)
    bool playerExists(const QString &login);

    // Получение игрока по ID
    PlayerData getPlayerById(int userId);

    // Получение игрока по логину
    PlayerData getPlayerByLogin(const QString &login);

    // Получение всех игроков (сортировка по очкам — аналог sortByPointsDesc)
    QList<PlayerData> getAllPlayers(bool sortByPointsDesc = false);

    // Получение модели для отображения в QTableView
    QSqlTableModel* getTableModel();

    // Получение текста последней ошибки
    QString lastError() const;

signals:
    // Сигналы для уведомления GUI (вместо cout/cerr)
    void connected();
    void disconnected();
    void playerSaved(int playerId);
    void playerDeleted(int playerId);
    void errorOccurred(const QString &message);
    void infoMessage(const QString &message);

private:
    QSqlDatabase m_db;
    QString m_connectionName;

    // Вспомогательный метод: выполнить запрос и проверить ошибку
    bool execQuery(QSqlQuery &query, const QString &errorPrefix = "");
};

#endif // PLAYERDATABASE_H