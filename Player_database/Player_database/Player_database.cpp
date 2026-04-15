
#include <iostream>
#include <windows.h>
#include "include.h"
#include "Data_Player.h"





int main()
{
    setlocale(0, "Ru");
    std::cout << "Hello World!\n";
    sqlite3* bd_player;
    sqlite3_open("player_bd.db", &bd_player);
    
    Data_Player person1;

    //person1.setLogin("UFDLF");

    person1.deleteById(bd_player, 2);
    person1.saveToBD(bd_player);
    return 0;
    


}


