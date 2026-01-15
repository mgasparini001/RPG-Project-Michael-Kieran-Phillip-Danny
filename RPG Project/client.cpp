#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
#include "npc.h"
#include "character.h"
#include "player.h"
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

int enterBattle() {
	cout << "entered battle\n";

	npc npc1(5, 10, 3,  4,  7, 100);

	npc npc2(6, 10, 3, 4, 7, 100);

	npc npc3(7, 10, 3, 4, 7, 100);

	npc npc4(8, 10, 3, 4, 7, 100);

	int enemyRoll = diceRoll(3);

	cout << "Enemy #" << enemyRoll << endl;

	if (enemyRoll == 1) {
		cout << "Enemy HP: " << npc1.getHp() << endl;
		return npc1.getHp();
	}

	else if (enemyRoll == 2) {
		cout << "Enemy HP: " << npc2.getHp() << endl;
		return npc2.getHp();
	}

	else {
		cout << "Enemy HP: " << npc3.getHp() << endl;
		return npc3.getHp();
	}
	
	//while (






}
int main() {

	player p1();


	srand(static_cast<unsigned int>(time(0)));
	//cout << diceRoll(6);

	
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
			enterBattle();
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