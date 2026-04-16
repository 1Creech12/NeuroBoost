#include "databaseqt.h"

// ============================================
// КОНСТРУКТОР
// ============================================
PlayerDatabase::PlayerDatabase(QObject *parent)
    : QObject(parent)
{
    m_connectionName = QString("player_db_%1").arg((quintptr)this);
}

// ============================================
// ДЕСТРУКТОР
// ============================================
PlayerDatabase::~PlayerDatabase()
{
    disconnect();
}

// ============================================
// ПОДКЛЮЧЕНИЕ К БАЗЕ ДАННЫХ
// ============================================
bool PlayerDatabase::connect(const QString &dbPath)
{
    if (m_db.isOpen()) {
        disconnect();
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        emit errorOccurred("Ошибка открытия БД: " + m_db.lastError().text());
        return false;
    }

    emit connected();
    emit infoMessage("База данных открыта: " + dbPath);
    qDebug() << "[PlayerDatabase] Подключено к:" << dbPath;
    return true;
}

// ============================================
// ОТКЛЮЧЕНИЕ ОТ БД
// ============================================
void PlayerDatabase::disconnect()
{
    if (m_db.isOpen()) {
        m_db.close();
        emit disconnected();
        qDebug() << "[PlayerDatabase] Отключено от БД";
    }
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connectionName);
}

// ============================================
// ПРОВЕРКА ПОДКЛЮЧЕНИЯ
// ============================================
bool PlayerDatabase::isConnected() const
{
    return m_db.isOpen();
}

// ============================================
// СОЗДАНИЕ ТАБЛИЦЫ users
// ============================================
bool PlayerDatabase::createTable()
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            login TEXT NOT NULL UNIQUE,
            point INTEGER DEFAULT 0,
            diamonds INTEGER DEFAULT 0
        )
    )";

    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        emit errorOccurred("Ошибка создания таблицы: " + query.lastError().text());
        return false;
    }

    emit infoMessage("Таблица users создана/проверена");
    qDebug() << "[PlayerDatabase] Таблица users готова";
    return true;
}

// ============================================
// ВСПОМОГАТЕЛЬНЫЙ МЕТОД: ВЫПОЛНЕНИЕ ЗАПРОСА
// ============================================
bool PlayerDatabase::execQuery(QSqlQuery &query, const QString &errorPrefix)
{
    if (!query.exec()) {
        QString err = errorPrefix.isEmpty() ? "Ошибка SQL" : errorPrefix;
        err += ": " + query.lastError().text();
        emit errorOccurred(err);
        qDebug() << "[PlayerDatabase] " << err;
        return false;
    }
    return true;
}

// ============================================
// СОХРАНЕНИЕ ИГРОКА
// ============================================
bool PlayerDatabase::savePlayer(PlayerData &player)
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT id FROM users WHERE login = :login");
    checkQuery.bindValue(":login", player.login);

    if (!checkQuery.exec()) {
        emit errorOccurred("Ошибка проверки существования: " + checkQuery.lastError().text());
        return false;
    }

    bool exists = checkQuery.next();
    int existingId = exists ? checkQuery.value(0).toInt() : -1;

    if (player.id > 0 || (exists && existingId > 0)) {
        int updateId = (player.id > 0) ? player.id : existingId;

        QSqlQuery query(m_db);
        query.prepare(R"(
            UPDATE users
            SET name = :name,
                last_name = :last_name,
                point = :point,
                diamonds = :diamonds
            WHERE id = :id
        )");

        query.bindValue(":name", player.name);
        query.bindValue(":last_name", player.lastName);
        query.bindValue(":point", player.points);
        query.bindValue(":diamonds", player.diamonds);
        query.bindValue(":id", updateId);

        if (!query.exec()) {
            emit errorOccurred("Ошибка обновления: " + query.lastError().text());
            return false;
        }

        player.id = updateId;
        emit playerSaved(player.id);
        emit infoMessage(QString("Игрок обновлён: %1 %2 (ID: %3)")
                             .arg(player.name, player.lastName).arg(player.id));
        qDebug() << "[PlayerDatabase] Обновлён игрок ID:" << player.id;

        updateTop100();  // Автообновление топ-100
        return true;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO users (name, last_name, login, point, diamonds)
        VALUES (:name, :last_name, :login, :point, :diamonds)
    )");

    query.bindValue(":name", player.name);
    query.bindValue(":last_name", player.lastName);
    query.bindValue(":login", player.login);
    query.bindValue(":point", player.points);
    query.bindValue(":diamonds", player.diamonds);

    if (!query.exec()) {
        if (query.lastError().nativeErrorCode() == "19") {
            emit errorOccurred("Игрок с таким логином уже существует!");
        } else {
            emit errorOccurred("Ошибка вставки: " + query.lastError().text());
        }
        return false;
    }

    player.id = query.lastInsertId().toInt();
    emit playerSaved(player.id);
    emit infoMessage(QString("Новый игрок добавлен: %1 %2 (ID: %3)")
                         .arg(player.name, player.lastName).arg(player.id));
    qDebug() << "[PlayerDatabase] Добавлен игрок ID:" << player.id;

    updateTop100();  // Автообновление топ-100
    return true;
}

// ============================================
// УДАЛЕНИЕ ИГРОКА
// ============================================
bool PlayerDatabase::deletePlayer(int userId)
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    if (userId <= 0) {
        emit errorOccurred("Неверный ID игрока");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (!query.exec()) {
        emit errorOccurred("Ошибка удаления: " + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() > 0) {
        emit playerDeleted(userId);
        emit infoMessage(QString("Игрок с ID %1 удалён").arg(userId));
        qDebug() << "[PlayerDatabase] Удалён игрок ID:" << userId;
        updateTop100();  // Автообновление топ-100
        return true;
    } else {
        emit errorOccurred(QString("Игрок с ID %1 не найден").arg(userId));
        return false;
    }
}

// ============================================
// ПРОВЕРКА СУЩЕСТВОВАНИЯ ИГРОКА
// ============================================
bool PlayerDatabase::playerExists(const QString &login)
{
    if (!isConnected()) return false;

    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM users WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    return query.next();
}

// ============================================
// ПОЛУЧЕНИЕ ИГРОКА ПО ID
// ============================================
PlayerData PlayerDatabase::getPlayerById(int userId)
{
    PlayerData player;

    if (!isConnected() || userId <= 0) {
        return player;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, last_name, login, point, diamonds FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    query.exec();

    if (query.next()) {
        player.id = query.value(0).toInt();
        player.name = query.value(1).toString();
        player.lastName = query.value(2).toString();
        player.login = query.value(3).toString();
        player.points = query.value(4).toInt();
        player.diamonds = query.value(5).toInt();
    }

    return player;
}

// ============================================
// ПОЛУЧЕНИЕ ИГРОКА ПО ЛОГИНУ
// ============================================
PlayerData PlayerDatabase::getPlayerByLogin(const QString &login)
{
    PlayerData player;

    if (!isConnected()) {
        return player;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, last_name, login, point, diamonds FROM users WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    if (query.next()) {
        player.id = query.value(0).toInt();
        player.name = query.value(1).toString();
        player.lastName = query.value(2).toString();
        player.login = query.value(3).toString();
        player.points = query.value(4).toInt();
        player.diamonds = query.value(5).toInt();
    }

    return player;
}

// ============================================
// ПОЛУЧЕНИЕ ВСЕХ ИГРОКОВ
// ============================================
QList<PlayerData> PlayerDatabase::getAllPlayers(bool sortByPointsDesc)
{
    QList<PlayerData> players;

    if (!isConnected()) {
        return players;
    }

    QString sql = "SELECT id, name, last_name, login, point, diamonds FROM users";
    if (sortByPointsDesc) {
        sql += " ORDER BY point DESC";
    }

    QSqlQuery query(m_db);
    query.exec(sql);

    while (query.next()) {
        PlayerData player;
        player.id = query.value(0).toInt();
        player.name = query.value(1).toString();
        player.lastName = query.value(2).toString();
        player.login = query.value(3).toString();
        player.points = query.value(4).toInt();
        player.diamonds = query.value(5).toInt();
        players.append(player);
    }

    return players;
}

// ============================================
// ПОЛУЧЕНИЕ МОДЕЛИ ДЛЯ QTableView
// ============================================
QSqlTableModel* PlayerDatabase::getTableModel()
{
    if (!isConnected()) {
        return nullptr;
    }

    QSqlTableModel *model = new QSqlTableModel(this, m_db);
    model->setTable("users");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Имя");
    model->setHeaderData(2, Qt::Horizontal, "Фамилия");
    model->setHeaderData(3, Qt::Horizontal, "Логин");
    model->setHeaderData(4, Qt::Horizontal, "Очки");
    model->setHeaderData(5, Qt::Horizontal, "Алмазы");

    return model;
}

// ============================================
// ПОЛУЧЕНИЕ ПОСЛЕДНЕЙ ОШИБКИ
// ============================================
QString PlayerDatabase::lastError() const
{
    return m_db.lastError().text();
}

// ============================================
// ДОБАВИТЬ ОЧКИ ИГРОКУ ПО ID
// ============================================
bool PlayerDatabase::addPoints(int userId, int pointsToAdd)
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    if (userId <= 0) {
        emit errorOccurred("Неверный ID игрока");
        return false;
    }

    int currentPoints = getPlayerPoints(userId);
    int newPoints = currentPoints + pointsToAdd;

    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET point = :points WHERE id = :id");
    query.bindValue(":points", newPoints);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        emit errorOccurred("Ошибка начисления очков: " + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() > 0) {
        emit infoMessage(QString("✅ Игрок ID %1: +%2 очков (теперь %3)")
                             .arg(userId).arg(pointsToAdd).arg(newPoints));
        qDebug() << "[PlayerDatabase] Игрок ID" << userId
                 << "получил" << pointsToAdd << "очков. Всего:" << newPoints;
        updateTop100();  // Автообновление топ-100
        return true;
    } else {
        emit errorOccurred(QString("Игрок с ID %1 не найден").arg(userId));
        return false;
    }
}

// ============================================
// ДОБАВИТЬ АЛМАЗЫ ИГРОКУ ПО ID
// ============================================
bool PlayerDatabase::addDiamonds(int userId, int diamondsToAdd)
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    if (userId <= 0) {
        emit errorOccurred("Неверный ID игрока");
        return false;
    }

    int currentDiamonds = getPlayerDiamonds(userId);
    int newDiamonds = currentDiamonds + diamondsToAdd;

    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET diamonds = :diamonds WHERE id = :id");
    query.bindValue(":diamonds", newDiamonds);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        emit errorOccurred("Ошибка начисления алмазов: " + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() > 0) {
        emit infoMessage(QString("💎 Игрок ID %1: +%2 алмазов (теперь %3)")
                             .arg(userId).arg(diamondsToAdd).arg(newDiamonds));
        qDebug() << "[PlayerDatabase] Игрок ID" << userId
                 << "получил" << diamondsToAdd << "алмазов. Всего:" << newDiamonds;
        updateTop100();  // Автообновление топ-100
        return true;
    } else {
        emit errorOccurred(QString("Игрок с ID %1 не найден").arg(userId));
        return false;
    }
}

// ============================================
// УСТАНОВИТЬ ТОЧНОЕ КОЛИЧЕСТВО ОЧКОВ
// ============================================
bool PlayerDatabase::setPoints(int userId, int newPoints)
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    if (userId <= 0) {
        emit errorOccurred("Неверный ID игрока");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET point = :points WHERE id = :id");
    query.bindValue(":points", newPoints);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        emit errorOccurred("Ошибка установки очков: " + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() > 0) {
        emit infoMessage(QString("🎯 Очки игрока ID %1 установлены: %2")
                             .arg(userId).arg(newPoints));
        updateTop100();  // Автообновление топ-100
        return true;
    } else {
        emit errorOccurred(QString("Игрок с ID %1 не найден").arg(userId));
        return false;
    }
}

// ============================================
// УСТАНОВИТЬ ТОЧНОЕ КОЛИЧЕСТВО АЛМАЗОВ
// ============================================
bool PlayerDatabase::setDiamonds(int userId, int newDiamonds)
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    if (userId <= 0) {
        emit errorOccurred("Неверный ID игрока");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET diamonds = :diamonds WHERE id = :id");
    query.bindValue(":diamonds", newDiamonds);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        emit errorOccurred("Ошибка установки алмазов: " + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() > 0) {
        emit infoMessage(QString("💎 Алмазы игрока ID %1 установлены: %2")
                             .arg(userId).arg(newDiamonds));
        updateTop100();  // Автообновление топ-100
        return true;
    } else {
        emit errorOccurred(QString("Игрок с ID %1 не найден").arg(userId));
        return false;
    }
}

// ============================================
// ПОЛУЧИТЬ ТЕКУЩИЕ ОЧКИ ИГРОКА
// ============================================
int PlayerDatabase::getPlayerPoints(int userId)
{
    if (!isConnected() || userId <= 0) {
        return 0;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT point FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// ============================================
// ПОЛУЧИТЬ ТЕКУЩИЕ АЛМАЗЫ ИГРОКА
// ============================================
int PlayerDatabase::getPlayerDiamonds(int userId)
{
    if (!isConnected() || userId <= 0) {
        return 0;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT diamonds FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// ============================================
// ТОП-20 ПО ОЧКАМ
// ============================================
QList<PlayerData> PlayerDatabase::getTop20ByPoints()
{
    return getTopPlayersByPoints(20);
}

QList<PlayerData> PlayerDatabase::getTopPlayersByPoints(int limit)
{
    QList<PlayerData> players;

    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return players;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, name, last_name, login, point, diamonds
        FROM users
        ORDER BY point DESC
        LIMIT :limit
    )");
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        emit errorOccurred("Ошибка получения топа по очкам: " + query.lastError().text());
        return players;
    }

    while (query.next()) {
        PlayerData player;
        player.id = query.value(0).toInt();
        player.name = query.value(1).toString();
        player.lastName = query.value(2).toString();
        player.login = query.value(3).toString();
        player.points = query.value(4).toInt();
        player.diamonds = query.value(5).toInt();
        players.append(player);
    }

    return players;
}

// ============================================
// ТОП-20 ПО АЛМАЗАМ
// ============================================
QList<PlayerData> PlayerDatabase::getTop20ByDiamonds()
{
    return getTopPlayersByDiamonds(20);
}

QList<PlayerData> PlayerDatabase::getTopPlayersByDiamonds(int limit)
{
    QList<PlayerData> players;

    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return players;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, name, last_name, login, point, diamonds
        FROM users
        ORDER BY diamonds DESC
        LIMIT :limit
    )");
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        emit errorOccurred("Ошибка получения топа по алмазам: " + query.lastError().text());
        return players;
    }

    while (query.next()) {
        PlayerData player;
        player.id = query.value(0).toInt();
        player.name = query.value(1).toString();
        player.lastName = query.value(2).toString();
        player.login = query.value(3).toString();
        player.points = query.value(4).toInt();
        player.diamonds = query.value(5).toInt();
        players.append(player);
    }

    return players;
}

// ============================================
// СОЗДАТЬ ТАБЛИЦУ ТОП-100
// ============================================
bool PlayerDatabase::createTop100Table()
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS top100 (
            rank INTEGER PRIMARY KEY,
            user_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            login TEXT NOT NULL,
            points INTEGER DEFAULT 0,
            diamonds INTEGER DEFAULT 0,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";

    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        emit errorOccurred("Ошибка создания таблицы top100: " + query.lastError().text());
        return false;
    }

    emit infoMessage("Таблица top100 создана/проверена");
    qDebug() << "[PlayerDatabase] Таблица top100 готова";
    return true;
}

// ============================================
// ОЧИСТИТЬ ТАБЛИЦУ ТОП-100
// ============================================
bool PlayerDatabase::clearTop100()
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec("DELETE FROM top100")) {
        emit errorOccurred("Ошибка очистки top100: " + query.lastError().text());
        return false;
    }

    qDebug() << "[PlayerDatabase] Таблица top100 очищена";
    return true;
}

// ============================================
// ОБНОВИТЬ ТОП-100
// ============================================
bool PlayerDatabase::updateTop100()
{
    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return false;
    }

    if (!createTop100Table()) {
        return false;
    }

    if (!clearTop100()) {
        return false;
    }

    QSqlQuery selectQuery(m_db);
    selectQuery.prepare(R"(
        SELECT id, name, last_name, login, point, diamonds
        FROM users
        ORDER BY point DESC
        LIMIT 100
    )");

    if (!selectQuery.exec()) {
        emit errorOccurred("Ошибка получения топ-100: " + selectQuery.lastError().text());
        return false;
    }

    QSqlQuery insertQuery(m_db);
    insertQuery.prepare(R"(
        INSERT INTO top100 (rank, user_id, name, last_name, login, points, diamonds)
        VALUES (:rank, :user_id, :name, :last_name, :login, :points, :diamonds)
    )");

    int rank = 1;
    int insertedCount = 0;

    while (selectQuery.next()) {
        insertQuery.bindValue(":rank", rank++);
        insertQuery.bindValue(":user_id", selectQuery.value(0).toInt());
        insertQuery.bindValue(":name", selectQuery.value(1).toString());
        insertQuery.bindValue(":last_name", selectQuery.value(2).toString());
        insertQuery.bindValue(":login", selectQuery.value(3).toString());
        insertQuery.bindValue(":points", selectQuery.value(4).toInt());
        insertQuery.bindValue(":diamonds", selectQuery.value(5).toInt());

        if (!insertQuery.exec()) {
            emit errorOccurred("Ошибка вставки в top100: " + insertQuery.lastError().text());
            return false;
        }
        insertedCount++;
    }

    emit infoMessage(QString("✅ Топ-100 обновлён! Сохранено %1 игроков").arg(insertedCount));
    qDebug() << "[PlayerDatabase] Топ-100 обновлён. Сохранено:" << insertedCount << "игроков";
    return true;
}

// ============================================
// ПОЛУЧИТЬ СОХРАНЁННЫЙ ТОП-100
// ============================================
QList<PlayerData> PlayerDatabase::getSavedTop100()
{
    QList<PlayerData> players;

    if (!isConnected()) {
        emit errorOccurred("Нет подключения к БД");
        return players;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT user_id, name, last_name, login, points, diamonds, rank
        FROM top100
        ORDER BY rank ASC
    )");

    if (!query.exec()) {
        emit errorOccurred("Ошибка получения сохранённого топ-100: " + query.lastError().text());
        return players;
    }

    while (query.next()) {
        PlayerData player;
        player.id = query.value(0).toInt();
        player.name = query.value(1).toString();
        player.lastName = query.value(2).toString();
        player.login = query.value(3).toString();
        player.points = query.value(4).toInt();
        player.diamonds = query.value(5).toInt();
        players.append(player);
    }

    qDebug() << "[PlayerDatabase] Загружено из top100:" << players.size() << "игроков";
    return players;
}