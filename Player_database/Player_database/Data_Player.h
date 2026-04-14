#pragma once
#include "include.h"
class Data_Player
{
private:
	string name;
	string last_name;
	string login;
	int point;
	int id;
public:

	Data_Player();
	Data_Player(string name, string last_name, string login);





	//сеторы
	Data_Player& setName(string new_name);
	Data_Player& setLastName(string new_last_name);
	Data_Player& setLogin(string login);
	Data_Player& setPoint();

	//геторы
	string getName();
	string getLastName();
	string getLogin();
	string getPoint();


	//методы 

	bool saveToBD();



};

