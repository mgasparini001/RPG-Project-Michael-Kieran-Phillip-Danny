#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
#include "npc.h"
#include "character.h"
#include "player.h"
#include "enemy.h"
using namespace std;

void returnToOverworld() {
	cout << "returned to overworld\n";

}

void manageInventory() {
	cout << "managed inventory\n";
}

void enterShop() {
	cout << "entered shop\n";
}

void chat() {
	cout << "Talked to someone\n";
}
void quitGame() {
	cout << "quit\n";
}

//rolls a dice based on an input (x = 6 would mean a six sided die

int diceRoll(int x)
{
	return (rand() % x) + 1;
}

void enterBattle(enemy& enemy1,enemy& enemy2, enemy& enemy3, player& p1) {
	cout << "entered battle\n";

	

	int enemyRoll = diceRoll(3);

	cout << "Enemy #" << enemyRoll << endl;

	if (enemyRoll == 1) {
		cout << "Enemy HP: " << enemy1.getHp() << endl;
		cout << enemy1.getHp();

		while (p1.getHp() > 0 && enemy1.getHp() > 0) {


		}
	}

	else if (enemyRoll == 2) {
		cout << "Enemy HP: " << enemy2.getHp() << endl;
		cout << enemy2.getHp();

		while (p1.getHp() > 0 && enemy2.getHp() > 0) {


		}
	}

	else {
		cout << "Enemy HP: " << enemy3.getHp() << endl;
		cout << enemy3.getHp();

		while (p1.getHp() > 0 && enemy3.getHp() > 0) {


		}
	}

	
}






int main() {
	
	player p1("Ash", 5, 8, 4, 3, 6, 7, 1);
	
	enemy enemy1("dragon", 1, 3, 4, 7, 2, 5, 10, 100);

	enemy enemy2("zombie", 1, 6, 4, 7, 2, 5, 10, 100);

	enemy enemy3("shrek", 1, 5, 4, 7, 2, 5, 10, 100);

	
	srand(static_cast<unsigned int>(time(0)));
	
	
	bool hasQuit = false;
	while (!hasQuit) {
		
	string menu = R"(MENU:

1. Return to Game
2. Enter Battle
3. Manage Inventory
4. Enter Shop
5. Talk to Someone
6. Quit Game

)";

		cout << menu;
		int num;
		cin >> num;

		switch (num) {
		case 1:
			returnToOverworld();
			break;
		case 2:
			enterBattle(enemy1, enemy2, enemy3, p1);
			hasQuit = true;
			break;
			
		case 3:
			manageInventory();
			break;
		case 4:
			enterShop();
			break;
		case 5:
			chat();
			break;
		case 6:
			quitGame();
			hasQuit = true;
			break;




		}

	}


		 



	return 0;
}