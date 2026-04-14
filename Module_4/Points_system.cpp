#include "Points_system.h"
#include "includes.h"



Points_system::Points_system() {
	sqlite3* database;
	char* errMsg = nullptr;

	cout << "Введите имя свое";


	sqlite3_open("database_player", &database);
}


Points_system& Points_system::setPoint(int* point) {
	this->point = point;
	return *this;
}

Points_system& Points_system::setPenalization(int* penalization) {
	this->penalization = penalization;
	return *this;
}
Points_system& Points_system::setCombo(int* combo) {
	this->combo = combo;
	return *this;
}


int Points_system::getPoint() { return *point; }
int Points_system::getPenalization() { return *penalization; }
int Points_system::getCombo() { return *combo; }




int Points_system::viewing_glasses() {
	sqlite3* db;
	char* errMsg = nullptr;

	sqlite3_open("new.db", &db);

	const char* sqlCreate = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            age INTEGER
        );
    )";


	const char* sql = "INSERT INTO users (name, age) VALUES ('Иван',25);";

	int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		std::cerr << "Ошибка вставки: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	else {
		std::cout << "Пользователь добавлен!" << std::endl;
	}

	sqlite3_close(db);



	return 0;



}





