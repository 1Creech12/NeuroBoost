#include "Data_Player.h"
#include "include.h"
#include <windows.h>



// Функция конвертации CP1251 -> UTF-8

string cp1251_to_utf8(const string& cp1251_str) {
    if (cp1251_str.empty()) return {};

    // 1. Определяем размер буфера для UTF-16 строки
    int utf16_len = MultiByteToWideChar(1251, 0, cp1251_str.c_str(), -1, nullptr, 0);
    if (utf16_len == 0) return {};

    // 2. Конвертируем CP1251 -> UTF-16
    wstring utf16_str(utf16_len, 0);
    MultiByteToWideChar(1251, 0, cp1251_str.c_str(), -1, &utf16_str[0], utf16_len);

    // 3. Определяем размер буфера для UTF-8 строки
    int utf8_len = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8_len == 0) return {};

    // 4. Конвертируем UTF-16 -> UTF-8
    string utf8_str(utf8_len, 0);
    WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), -1, &utf8_str[0], utf8_len, nullptr, nullptr);

    // Удаляем завершающий нулевой символ
    if (!utf8_str.empty() && utf8_str.back() == '\0') {
        utf8_str.pop_back();
    }
    return utf8_str;
}







Data_Player::Data_Player(): id(-1),name (""), last_name(""), login(""), point (0), diamonds(0) {}

Data_Player::Data_Player(string name, string last_name, string login): name(name), last_name(last_name), login (login), point (0), diamonds(0), id (-1) {

    sqlite3* bd_player;
    char* errMsg = nullptr;
    int rc;

    // Конвертируем строки в UTF-8
    string utf8_name = cp1251_to_utf8(name);
    string utf8_last_name = cp1251_to_utf8(last_name);
    string utf8_login = cp1251_to_utf8(login);

    const char* sqlCreate = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            login TEXT NOT NULL,
            point INTEGER DEFAULT 0,
            diamonds INTEGER DEFAULT 0
        );
    )";

    sqlite3_open("player_bd.db", &bd_player);
    sqlite3_exec(bd_player, sqlCreate, nullptr, nullptr, &errMsg);


    const char* sqlInsert = "INSERT INTO users (name, last_name, login, point, diamonds) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(bd_player, sqlInsert, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(bd_player) << endl;
        sqlite3_close(bd_player);
        return ;
    }

    // Привязываем только данные из параметров конструктора
    sqlite3_bind_text(stmt, 1, utf8_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, utf8_last_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, utf8_login.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, point);
    sqlite3_bind_int(stmt, 5, diamonds);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Ошибка вставки: " << sqlite3_errmsg(bd_player) << endl;
    }
    else {
        cout << "Данные успешно добавлены!" << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(bd_player);


}


Data_Player& Data_Player::setName(string new_name) {
	this->name = new_name;
	return *this;
}
Data_Player& Data_Player::setLastName(string new_last_name) {
	this->last_name= new_last_name;
	return *this;
}
Data_Player& Data_Player::setLogin(string login) {
	this->login = login;
	return *this;
}
//Data_Player& Data_Player::setName(string new_name) {
//	this->name = new_name;
//	return *this;
//}
bool Data_Player::saveToBD(sqlite3* db) {
    if (db == nullptr) {
        std::cerr << "Ошибка: БД не открыта" << endl;
        return false;
    }


    char* errMsg = nullptr;
    int rc;



    const char* sqlCreate = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            login TEXT NOT NULL UNIQUE,
            point INTEGER DEFAULT 0,
            diamonds INTEGER DEFAULT 0
    );
)";

    rc = sqlite3_exec(db, sqlCreate, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    const char* sqlCheck = "SELECT id FROM users WHERE login = ?;"; 
    sqlite3_stmt* checkStmt;

    rc = sqlite3_prepare_v2(db, sqlCheck, -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки проверки" << sqlite3_errmsg(db) << endl;
        return false;
    }




    std::string utf8_login = cp1251_to_utf8(login);
    sqlite3_bind_text(checkStmt, 1, utf8_login.c_str(), -1, SQLITE_TRANSIENT);

    bool userExists = (sqlite3_step(checkStmt) == SQLITE_ROW);
    sqlite3_finalize(checkStmt);

    // 3. ЕСЛИ ПОЛЬЗОВАТЕЛЬ СУЩЕСТВУЕТ → ОБНОВЛЯЕМ
    if (userExists && id != -1) {
        const char* sqlUpdate = "UPDATE users SET name = ?, last_name = ?, point = ?, diamonds = ? WHERE id = ?;";
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, sqlUpdate, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Ошибка подготовки UPDATE: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        std::string utf8_name = cp1251_to_utf8(name);
        std::string utf8_last_name = cp1251_to_utf8(last_name);

        sqlite3_bind_text(stmt, 1, utf8_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, utf8_last_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, point);
        sqlite3_bind_int(stmt, 4, diamonds);
        sqlite3_bind_int(stmt, 5, id);

        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc == SQLITE_DONE) {
            std::cout << "Данные обновлены для " << name << " " << last_name << std::endl;
            return true;
        }
        else {
            std::cerr << "Ошибка обновления: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
    }

    else {
        const char* sqlInsert = "INSERT INTO users (name, last_name, login, point, diamonds) VALUES (?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Ошибка подготовки INSERT: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        std::string utf8_name = cp1251_to_utf8(name);
        std::string utf8_last_name = cp1251_to_utf8(last_name);

        sqlite3_bind_text(stmt, 1, utf8_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, utf8_last_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, utf8_login.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, point);
        sqlite3_bind_int(stmt, 5, diamonds);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE) {

            id = sqlite3_last_insert_rowid(db);
            sqlite3_finalize(stmt);
            std::cout << "Новый игрок добавлен! ID = " << id << std::endl;
            return true;
        }
        else {
            std::cerr << "Ошибка вставки: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
    }
}

bool Data_Player::deleteById(sqlite3* db, int userId) {
    if (db == nullptr) {
        std::cerr << "Ошибка: БД не открыта" << endl;
        return false;
    }


    if (id == -1) {
        std::cerr << "Ошибка: пользователь не сохранён в БД (id = -1)" << std::endl;
        return false;
    }

    const char* sql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        std::cout << "Пользователь " << name << " (ID: " << userId << ") удалён!" << std::endl;
        id = -1;  // Сбрасываем ID, так как пользователь больше не в БД
        return true;
    }
    else {
        std::cerr << "Ошибка удаления: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
}

