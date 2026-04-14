#include "Data_Player.h"
#include "include.h"




Data_Player::Data_Player(): id(-1),name (""), last_name(""), login(""),point (0) {}

Data_Player::Data_Player(string name, string last_name, string login): name(name), last_name(last_name), login (login), point (0) {

	sqlite3* bd_player;
	char* errMsg = nullptr;
	



	


	const char* sqlCreate = R"(
		CREATE TABLE IF NOT EXISTS users (
			id INTEGER PRIMARY KEY AUROINCREAMENT,
			name TEXT NOT NULL,
			last_name TEXT NOT NULL,
			login TEXT NOT NULL,
			point INTEGER DEFAULT 0
		);
	)";


	sqlite3_open("player_bd.db", &bd_player);


	sqlite3_exec(bd_player, sqlCreate, nullptr, nullptr, &errMsg);

	const char* sqlInsert = R"(
    INSERT INTO users (name, last_name, login, point) VALUES 
        ('Иван', 'Петров', 'ivan123', 100),
        ('Мария', 'Иванова', 'maria456', 150),
        ('Петр', 'Сидоров', 'petr789', 75);
)";

	rc = sqlite3_exec(bd_player, sqlInsert, nullptr, nullptr, &errMsg);

	if (rc != SQLITE_OK) {
		std::cerr << "Ошибка вставки: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	else {
		std::cout << "Данные успешно добавлены!" << std::endl;
	}

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

