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
#include "ItemRegistry.h"
#include "inventory.h"
#include <SFML/Audio.hpp>
#include <filesystem>
using namespace std;

// refresehs terminal screen
void refreshScreen();

// Input: an enemy and a player     Output: Player and enemy stats
// desc: prints out player and enemy in battle and an action prompt for player
void printBattleDisplay(enemy &enemy, player &p1, ItemRegistry registry) 
{
	cout << endl << "ENEMY:" << endl;
	cout << enemy.getName() << endl;
	cout << "\nHP: " << enemy.getHp() << endl;
	cout << R"(



-----------------------------------------
)";

	cout << "\n\t\t\t" << "PLAYER:";
	cout << "\n\t\t\t" << p1.getName() << endl << endl;
	cout << "\t\t\t" << "HP: " << p1.getHp() << endl;
	cout << "\t\t\t" << "SP: " << p1.getStamina() << endl;
	cout << "\t\t\t" << "AP: " << p1.getAp() << endl;
	cout << "\t\t\t" << "DMG: " << p1.getDmg() << endl;

	// WIP player stats display 
	//cout << "\t\t\t" << "TOUGH: " << p1.getArmor() << endl;
	// cout << "\t\t\t" << "MOVE: " << p1.getMove() << endl;
	// cout << "\t\t\t" << "DEX: " << p1.getDex() << endl;
	// cout << "\t\t\t" << "WITS: " << p1.getWits() << endl;
	cout <<
		R"(
                       ---------------------
		      | 1. FIGHT   3. RUN   |
		      | 2. REST    4. ITEM  |
                       ---------------------
)";
	if (p1.getHasItemEquipped() == true)
	{
		std::cout << "\t\t\tEquipped: " << registry.getItemName(p1.getEquippedItemID()) << std::endl << std::endl;
	}
}

void returnToOverworld() {
	cout << "returned to overworld\n";

}

void manageInventory(player& p1, ItemRegistry& registry) {
	
	bool inInv = true;
	bool isEquipped = false;
	while (inInv)
	{
		//WIP inv screen
		refreshScreen();
		cout << "\n    INVENTORY    ";
		cout << "\n  Player: " << p1.getName() << "\n\n";
		
		cout << R"(
============================
|  1. Add Item             |
|  2. Remove Item          |
|  3. View Inventory       |
|  4. Equip Item           |
|  5. Exit                 |
============================
)";
		
		if (p1.getHasItemEquipped() == true)
		{
			std::cout << "\n\t\t\tEquipped: " << registry.getItemName(p1.getEquippedItemID()) << std::endl << std::endl;
		}
		int choice;
		cin >> choice;

		if (choice == 1)
		{
			refreshScreen();
			cout << "\aitems:\n";
			registry.printRegistry();

			int itemID, quantity;
			cout << "enter item ID: ";
			cin >> itemID;
			cout << "enter quantity: ";
			cin >> quantity;

			p1.addItemToInventory(itemID, quantity);
			cout << "Added " << quantity << " of " << registry.getItemName(itemID) << "!\n";
			cout << "press enter";
			cin.ignore();
			cin.get();
		}
		else if (choice == 2)
		{
			refreshScreen();
			cout << "\n";
			cout << "\aitems:\n";
			registry.printRegistry();
			cout << endl;
			p1.getInventory().printInventory(registry, p1);

			int itemID, quantity;
			cout << "enter item ID to remove: ";
			cin >> itemID;
			cout << "enter quantity: ";
			cin >> quantity;

			if (p1.removeItemFromInventory(itemID, registry, p1, quantity))
			{
				cout << "Removed " << quantity << " of " << registry.getItemName(itemID) << "!\n";
			}
			else
			{
				cout << "Item not found!\n";
			}
			cout << "press enter";
			cin.ignore();
			cin.get();
		}
		else if (choice == 3)
		{
			refreshScreen();
			cout << "\n";
			p1.getInventory().printInventory(registry, p1);
			cout << "press enter";
			cin.ignore();
			cin.get();
		}
		else if (choice == 5)
		{
			inInv = false;
			refreshScreen();
		}
		else if (choice == 4)
		{
			refreshScreen();
			cout << "\aitems:\n";
			registry.printRegistry();
			cout << endl;
			p1.getInventory().printInventory(registry, p1);
			int itemID;
			cout << "enter item ID: ";
			cin >> itemID;
			while (itemID < 0 || itemID > 3)
			{
				cout << "Outside input range, womp womp\n";
			}
			if (p1.equipItem(itemID, registry))
			{
				isEquipped = true;
				cout << "Equipped " << registry.getItemName(itemID) << "!\n";
			}
			else
			{
				cout << "you cant equip an item you dont have, dingus\n";
			}

			cout << "press enter";
			cin.ignore();
			cin.get();

		}
		else
		{
			cout << "invalid option womp womp\n";
		}
	}
	refreshScreen();
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

void debugMenu(player& p1, ItemRegistry& registry) 
{
	
}


// Input: int that represents the sided dice you want to roll    Output: the number you get after rolling
// desc: rolls a dice based on an input (x = 6 would mean a six sided die)
int diceRoll(int x)
{
	return (rand() % x) + 1;
}

// refreshes terminal screen
void refreshScreen() {
	system("cls");
}


// Input: an enemy and a player    Output: an interactive battle loop that reverts to main menu upon ending
// desc: displays enemy name and hp, and starts battle sequence loop, letting user either fight, rest, or flee. uses Character hp, stamina, dmg, and armor penetration (strength)
// lets player decide on melee or ranged attack, with ranged having a chance to miss but taking less stamina
void enterBattle(enemy& enemy, player& p1, ItemRegistry registry) {
	sf::Music Music;
	bool hasRun = false;

	cout << "\nentered battle...\n\n";

	cout << "A wild " << enemy.getName() << " appears!" << endl;

	string anything = "";
	cout << "Enter anything to proceed\n";
	cin >> anything;

	refreshScreen();
	
	Music.openFromFile("Placeholder_song.wav");

	Music.setLooping(true);

	Music.play();

	int choice;

	// Gameplay loop 
	do 
	{
		printBattleDisplay(enemy, p1, registry);
		cin >> choice;

		//valid input checker
		while (choice < 1 || choice > 4)
		{
			cout << "Invalid option, please enter a valid input\n";
			cin >> choice;
		}

	


		switch (choice) 
		{
			//case where the player decides to attack
			case 1:
			{
				int choice;
				cout << "1. Melee Attack (Costs 100 stamina)" << endl;
				cout << "2. Ranged Attack (Costs 50 stamina)" << endl;
				cin >> choice;
				while (choice < 1 || choice > 2)
				{
					cout << "Invalid option, please enter a valid input";
					cin >> choice;
				}
				if (choice == 1)
				{
					p1.attack(enemy, true);
				}
				else
				{
					p1.attack(enemy, false);
				}
				cout << "Enter anything to proceed\n";
				cin >> anything;
				refreshScreen();
				break;
			}
			//case where the player decides to rest
			case 2:
			{
				cout << p1.getName() << " rests and restores some stamina!" << endl;
				p1.rest();
				cout << "Enter anything to proceed\n";
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
				cout << "Enter anything to proceed\n";
				cin >> anything;
				refreshScreen();
				break;
			}
			//case where player uses an item (WIP)
			case 4:
			{
				manageInventory(p1, registry);
				break;
			}
		}
		printBattleDisplay(enemy, p1, registry);
	
		if (enemy.getHp() > 0 && !hasRun)
		{
			// Enemy Action Engine:

			int eRoll = enemy.diceRoll(10);
			//case where the enemy attacks
			if (eRoll <= 6)
			{
				enemy.attack(p1, true);
				cout << "Enter anything to proceed\n";
				cin >> anything;
				refreshScreen();
			}
			//case where the enemy tries to rest
			else if (eRoll <= 9)
			{
				cout << enemy.getName() << " rests and restores some stamina!" << endl;
				enemy.rest();
				cout << "Enter anything to proceed\n";
				cin >> anything;
				refreshScreen();
			}
			// case where enemy attempts to flee
			else {
				bool check = enemy.flee();
				if (check)
				{
					hasRun = true;
				}
				cout << "Enter anything to proceed";
				cin >> anything;
				refreshScreen();
			}
		}
		// ends once a party dies or escapes
	} while ((p1.getHp() > 0 && enemy.getHp() > 0) && !hasRun);
	
	Music.stop();

}


int main(){
	
	cout << filesystem::current_path() << endl;
	// the users player
	player p1("Ash", 50, 8, 4, 3, 1000, 6, 7);
	
	// 3 enemy types that can be fought
	fodder zombie("zombie", 50, 3, 4, 3, 1000, 6, 7);

	heavy dragon("dragon", 50, 6, 4, 7, 1000, 5, 6);

	boss shrek("shrek", 50, 5, 4, 10, 1000, 9, 6);

	// really basic item registry for testing
	ItemRegistry itemRegistry;
	

	itemRegistry.setItemName(0, "Iron Sword");
	itemRegistry.setItemName(1, "Health Potion");
	itemRegistry.setItemName(2, "Gold Coins");
	itemRegistry.setItemName(3, "Shield");
	
	// add some items to the player for testing
	p1.addItemToInventory(0, 1);  // 1 sword
	p1.addItemToInventory(1, 5);  // 5 health potions
	p1.addItemToInventory(2, 50); // 50 gold
	p1.addItemToInventory(3, 1); // 1 shield
	//setting dice seed
	srand(static_cast<unsigned int>(time(0)));
	//test commit
	
	bool hasQuit = false;

	while (!hasQuit) {
		
	string menu = R"(OVERWORLD
                        =======================  |
                        | 1. Return to Game   |  |
                        |                     |  |
                        =======================  |
                        | 2. Enter Battle     |  |
                        |                     |  |
                        =======================  |
                        | 3. Manage Inventory |  |
                        |                     |  |
                        =======================  |
                        | 4. Enter Shop       |  |
                        |                     |  |
                        =======================  |
                        | 5. Talk to Someone  |  |
                        |                     |  |
                        =======================  |
                        | 6. Quit Game        |  |
                        =======================  |
                                                 |
                                                 |
                                                 |
                                                 |
                                                 |
                                                 |
--------------------------------------------------
)";

		cout << menu;
		int num;
		cin >> num;

		while (num < 1 || num > 7)
		{
			cout << "Invalid option, please enter a valid input\n";
			cin >> num;
		}

		switch (num) {

		case 1:
			returnToOverworld();
			break;

		case 2:
		{
			//roll decides which enemy to enter battle with
			int roll = diceRoll(3);
			if (roll == 3)
			{
				enterBattle(zombie, p1, itemRegistry);
			}

			else if (roll == 2)
			{
				enterBattle(dragon, p1, itemRegistry);
			}

			else
			{
				enterBattle(shrek, p1, itemRegistry);
			}
			
			//after battle ends, refreshes and goes back to menu
			//refreshScreen();
			hasQuit = true; //try to implement it reverting to main menu later. must figure out out to reset the player stats after battle tho. Maybe just use setters?
			break;
			
		}
		case 3:
			manageInventory(p1, itemRegistry);
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