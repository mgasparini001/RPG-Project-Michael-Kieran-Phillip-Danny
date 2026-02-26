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
#include "Store.h"
#include <SFML/Audio.hpp>
#include "battle.h"
//#include <filesystem>
using namespace std;


void refreshScreen() {
	system("cls");
}


// Input: an enemy and a player     Output: Player and enemy stats
// desc: prints out player and enemy in battle and an action prompt for player

void printBattleDisplay(enemy& enemy, player& p1, const ItemRegistry& registry)
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



void enterShop(store s, player& p1, npc& o) {
	s.enterStore(p1, o);
}

void chat() {
	cout << "Talked to someone\n";
}
void quitGame() {
	cout << "quit\n";
}


// Input: int that represents the sided dice you want to roll    Output: the number you get after rolling
// desc: rolls a dice based on an input (x = 6 would mean a six sided die)

int diceRoll(int x)
{
	return (rand() % x) + 1;
}


int main(){
	//cout << filesystem::current_path() << endl;
	
	// the users player
	player p1("Ash", 50, 8, 4, 3, 1000, 6, 7, 1000, 1000);

	npc owner1("Bob", "Hes just bob", 50, 8, 4, 3, 1000, 6, 7, 1000, 1000);

	// 3 enemy types that can be fought
	fodder zombie("zombie", 50, 3, 4, 3, 1000, 6, 7);

	heavy dragon("dragon", 50, 6, 4, 7, 1000, 5, 6);

	boss shrek("shrek", 50, 5, 4, 10, 1000, 9, 6);

	// really basic item registry for testing
	ItemRegistry itemRegistry;
	Inventory inventory;
	auto i1 = std::make_shared<Item>(0, "Iron Sword", "Sword of Iron", 50, false);
	auto i2= std::make_shared<Item>(1, "Health Potion", "Potion of Health", 10, true);
	auto i3 = std::make_shared<Item>(2, "Shield", "Blocks stuff IDK", 100, false);
	auto i4 = std::make_shared<Item>(3, "Water Gun", "Gun of Water", 5000, false);
	auto i5 = std::make_shared<Item>(4, "Ice beam", "Beam of ice", 2, false);
	auto i6 = std::make_shared<Item>(5, "Hyper Beam", "Beam of Hyper", 500000000, false);
	
	
	itemRegistry.setItem(0, i1);
	itemRegistry.setItem(1, i2);
	itemRegistry.setItem(2, i3);
	itemRegistry.setItem(3, i4);
	itemRegistry.setItem(4, i5);
	itemRegistry.setItem(5, i6);
	

	store store1("Danny's stuff", itemRegistry);


	// add some items to the player for testing
	p1.addItemToInventory(0, itemRegistry, 1);  // 1 sword
	p1.addItemToInventory(1, itemRegistry, 5);  // 5 health potions
	p1.addItemToInventory(2, itemRegistry, 1); // 1 shield
	p1.addItemToInventory(3, itemRegistry, 1); // 1 watergun
	p1.addItemToInventory(4, itemRegistry, 1); // 1 ice beam weapon
	p1.addItemToInventory(5, itemRegistry, 1); // 1 Hyper Beam weapon
	
	
	owner1.addItemToInventory(0, itemRegistry, 1);
	owner1.addItemToInventory(1, itemRegistry, 5);
	owner1.addItemToInventory(2, itemRegistry, 1);
	owner1.addItemToInventory(3, itemRegistry, 1);
	owner1.addItemToInventory(4, itemRegistry, 1);
	owner1.addItemToInventory(5, itemRegistry, 1);
	//owner1.addItemToInventory(6, itemRegistry, 1);

	//setting dice seed
	srand(static_cast<unsigned int>(time(0)));
	
	
	bool hasQuit = false;
	sf::Music m;
	m.openFromFile("Main Menu.wav");
	m.setLooping(true);
	while (!hasQuit) {
		m.play();
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

		while (num < 1 || num > 6)
		{
			cout << "Invalid option, please enter a valid input\n";
			cin >> num;
		}

		switch (num) {

		case 1:
			returnToOverworld();
			m.stop();
			break;

		case 2:
		{
			m.stop();
			//roll decides which enemy to enter battle with
			int roll = diceRoll(3);
			if (roll == 3)
			{
				enterBattle(zombie, p1, itemRegistry, inventory);
			}

			else if (roll == 2)
			{
				enterBattle(dragon, p1, itemRegistry, inventory);
			}

			else
			{
				enterBattle(shrek, p1, itemRegistry, inventory);
			}
			
			//after battle ends, refreshes and goes back to menu
			//refreshScreen();
			hasQuit = true; //try to implement it reverting to main menu later. must figure out out how to reset the player stats after battle tho. Maybe just use setters?
			break;
			
		}
		case 3:
			m.stop();
			inventory.manageInventory(p1, itemRegistry);
			break;
		case 4:
			m.stop();
			enterShop(store1, p1, owner1);
			break;
		case 5:
			m.stop();
			chat();
			break;
		case 6:
			quitGame();
			hasQuit = true;
			m.stop();
			break;


		}
	}


		 



	return 0;
}