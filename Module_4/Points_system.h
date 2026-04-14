#pragma once
class Points_system
{
private:
	int* point; // все очки 
	int* penalization; // штрафные очки
	int* combo; // колчичество комбо
public:

	Points_system();
	// сеторы




	Points_system& setPoint(int* point);
	Points_system& setPenalization(int* penalization);
	Points_system& setCombo(int* combo);

	//геторы

	int getPoint();
	int getPenalization();
	int getCombo();

	// методы 

	
	int viewing_glasses();

	int correctAnswers();
		
	


};

