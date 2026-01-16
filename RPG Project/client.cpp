#include <iostream>
#include <cstdlib>
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

//refreshes terminal screen
void refreshScreen() {
	system("cls");
}


//rolls for enemy type, displays enemy name and hp, and starts battle sequence loop
void enterBattle(enemy& enemy1,enemy& enemy2, enemy& enemy3, player& p1) {

	cout << "entered battle\n\n";

	int enemyRoll = diceRoll(3);

	// (not really needed, just for testing) cout << "Enemy #" << enemyRoll << endl;

	if (enemyRoll == 1) {
		cout << "A wild " << enemy1.getName() << " appears!" << endl;
		cout << "HP: " << enemy1.getHp() << endl;
	
	}

	else if (enemyRoll == 2) {
		cout << "A wild " << enemy2.getName() << " appears!" << endl;
		cout << "HP: " << enemy2.getHp() << endl;

	}

	else {
		cout << "A wild " << enemy3.getName() << " appears!" << endl;
		cout << "HP: " << enemy3.getHp() << endl;

	}
	
	int choice;

	// Gameplay loop WIP 
	do {
		cout << "\n\n\n\n\n\n\n\n\t\t\t" << p1.getName() << endl;
		cout << "\t\t\t" << "HP: " << p1.getHp() << endl;
		cout << "\t\t\t" << "STR: " << p1.getStr() << endl;
		cout << "\t\t\t" << "TOUGH: " << p1.getToughness() << endl;
		cout << "\t\t\t" << "DMG: " << p1.getDmg() << endl;
		// WIP player stats display (need these player getter methods made)
		// cout << "\t\t\t" << "MOVE: " << p1.getMove() << endl;
		// cout << "\t\t\t" << "DEX: " << p1.getDex() << endl;
		// cout << "\t\t\t" << "WITS: " << p1.getWits() << endl;

		cout << R"(
			1. FIGHT
			2. ITEM    3. RUN
)";
		cin >> choice;

		refreshScreen();


	}
	while (p1.getHp() > 0 && enemy1.getHp() > 0);
		

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