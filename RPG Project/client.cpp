#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include "npc.h"
#include "character.h"
#include "player.h"
#include "enemy.h"
#include "fodder.h"
#include "heavy.h"
#include "boss.h"
using namespace std;

void printBattleDisplay(enemy &enemy, player &p1) {
	cout << endl << "ENEMY:" << endl;
	cout << enemy.getName() << endl;
	cout << "\nHP: " << enemy.getHp() << endl;
	cout << R"(



-----------------------------------------
)";

	cout << "\n\t\t\t" << "PLAYER:";
	cout << "\n\t\t\t" << p1.getName() << endl << endl;
	cout << "\t\t\t" << "HP: " << p1.getHp() << endl;
	cout << "\t\t\t" << "AP: " << p1.getAp() << endl;
	cout << "\t\t\t" << "DMG: " << p1.getDmg() << endl;
	// WIP player stats display (might need these player getter methods made)
	//cout << "\t\t\t" << "TOUGH: " << p1.getArmor() << endl;
	// cout << "\t\t\t" << "MOVE: " << p1.getMove() << endl;
	// cout << "\t\t\t" << "DEX: " << p1.getDex() << endl;
	// cout << "\t\t\t" << "WITS: " << p1.getWits() << endl;
	cout <<
		R"(
                       -------------------
		      | 1. FIGHT          |
		      | 2. REST    3. RUN |
                       -------------------
)";
}

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
void enterBattle(enemy& enemy, player& p1) {
	
	bool hasRun = false;

	cout << "\nentered battle...\n\n";

	cout << "A wild " << enemy.getName() << " appears!" << endl;

	string anything = "";
	cout << "Enter anything to proceed";
	cin >> anything;

	refreshScreen();
	
	int choice;

	// Gameplay loop WIP 
	do {
		printBattleDisplay(enemy, p1);
		cin >> choice;
		//valid input checker
		while (choice < 1 || choice > 3)
		{
			cout << "Invalid option, please enter a valid input";
			cin >> choice;
		}

	

		switch (choice) 
		{
			//case where the player decides to attack
			case 1:
			{
				int choice;
				cout << "1. Melee Attack (Costs 100 stamina)";
				cout << "2. Ranged Attack (Costs 50 stamina)";
				cin >> choice;
				if (choice == 1)
				{
					p1.attack(enemy, true);
				}
				else
				{
					p1.attack(enemy, false);
				}
				cout << "Enter anything to proceed";
				cin >> anything;
				refreshScreen();
				break;
			}
			//case where the player decides to use an item
			case 2:
			{
				cout << p1.getName() << " rests and restores some stamina!" << endl;
				p1.rest();
				cout << "Enter anything to proceed";
				cin >> anything;
				refreshScreen();
				break;
			}
			//case where the player tries to run away
			case 3:
			{
				bool check = p1.flee();
				if (check)
				{
					hasRun = true;
				}
				cout << "Enter anything to proceed";
				cin >> anything;
				refreshScreen();
			}

		}
		printBattleDisplay(enemy, p1);
	
		if (enemy.getHp() > 0 && !hasRun)
		{
			int eRoll = enemy.diceRoll(10);
			//case where the enemy attacks
			if (eRoll <= 3)
			{
				enemy.attack(p1, true);
				cout << "Enter anything to proceed";
				cin >> anything;
				refreshScreen();
			}
			//case where the enemy tries to use an item
			else if (eRoll <= 6)
			{
				//enemy rests
				enemy.rest();
				cout << enemy.getName() + " rests." << endl;
				int eStamRoll = enemy.diceRoll(10);

				cout << "Enter anything to proceed";
				cin >> anything;
				refreshScreen();
			}
			else {
				//enemy flees
				//enemy.flee();
			}
		}
	} while ((p1.getHp() > 0 && enemy.getHp() > 0) && !hasRun);
		

}



int main() {
	
	player p1("Ash", 50, 8, 4, 3, 8, 6, 7);
	
	fodder zombie("zombie", 10, 3, 4, 7, 3, 6, 7);

	heavy dragon("dragon", 10, 6, 4, 7, 4, 5, 6);

	boss shrek("shrek", 10, 5, 4, 7, 5, 9, 6);

	
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
		{
			int roll = diceRoll(3);
			if (roll == 3)
			{
				enterBattle(zombie, p1);
			}
			else if (roll == 2)
			{
				enterBattle(dragon, p1);
			}
			else
			{
				enterBattle(shrek, p1);
			}
			
			// true breaks the main menu loop. once more menu choices are implemented, this can be changed
			hasQuit = true;
			break;
		}
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