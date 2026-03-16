#include "battle.h"
#include <iostream>
#include "Inventory.h"
#include <SFML/Audio.hpp>
#include "character.h"
#include "enemy.h"
#include "player.h"
#include "ItemRegistry.h"
#include "errorChecking.h"
using namespace std;

//forward declare
void refreshScreen();


// Input: an enemy and a player     Output: Player and enemy stats
// desc: prints out player and enemy in battle and an action prompt for player

void printBattleDisplay(enemy& enemy, player& p1, const ItemRegistry& registry)
{
	cout << endl << "ENEMY:" << endl;
	cout << enemy.getName() << endl;
	cout << "\nHP: " << enemy.getHp() << endl;
	cout << R"(



---------------------------------------------
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


void enterBattle(enemy& enemy, player& p1, ItemRegistry& registry, Inventory inv) {
	sf::SoundBuffer buff1("Attack.wav");
	sf::SoundBuffer buff2("eppy.wav");
	sf::SoundBuffer buff3("Scream.wav");
	sf::Sound sound1(buff1);
	sf::Sound sound2(buff2);
	sf::Sound sound3(buff3);
	bool hasRun = false;
	sf::Music encounter;
	encounter.openFromFile("Encounter.wav");
	encounter.setLooping(true);
	encounter.setVolume(50.f);
	encounter.play();
	refreshScreen();
	cout << "\nentered battle...\n\n";

	cout << "A wild " << enemy.getName() << " appears!" << endl;
	
	string anything = "";
	cout << "Enter anything to proceed\n";
	cin >> anything;
	encounter.stop();

	refreshScreen();

	sf::Music Music;
	Music.openFromFile("Placeholder_song.wav");
	Music.setVolume(50.f);
	Music.setLooping(true);

	Music.play();

	int choice;

	// Gameplay loop 
	do
	{
		printBattleDisplay(enemy, p1, registry);
		bool valid = false;
		while (!valid)
		{
			string test = getValidInput();
			if (test != "fail")
			{
				choice = stoi(test);
				if (choice < 1 || choice > 4)
				{
					cout << "Invalid input. Please enter one of the numbers on screen" << endl;
				}
				else
				{
					valid = true;
				}
			}
		}

	switch (choice)
		{
			//case where the player decides to attack
		case 1:
		{
			int choice;
			cout << "\n1. Melee Attack (Costs 100 stamina)" << endl;
			cout << "2. Ranged Attack (Costs 50 stamina)" << endl;
			bool valid = false;
			while (!valid)
			{
				string test = getValidInput();
				if (test != "fail")
				{
					choice = stoi(test);
					if (choice < 1 || choice > 2)
					{
						cout << "Invalid input. Please enter one of the numbers on screen" << endl;
					}
					else
					{
						valid = true;
					}
				}
			}
			if (choice == 1)
			{
				if (p1.attack(enemy, true))
				{
					sound1.play();
				}
			}
			else
			{
				if (p1.attack(enemy, false))
				{
					sound1.play();
				}
			}
			cout << "Enter anything to proceed\n";
			cin >> anything;
			refreshScreen();
			sound1.stop();
			break;
		}
		//case where the player decides to rest
		case 2:
		{
			cout << p1.getName() << " rests and restores some stamina!" << endl;
			p1.rest();
			sound2.play();
			cout << "Enter anything to proceed\n";
			cin >> anything;
			sound2.stop();
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
			sound3.play();
			cout << "Enter anything to proceed\n";
			cin >> anything;
			sound3.stop();
			refreshScreen();
			break;
		}
		//case where player uses an item (WIP-still need to make item stats and implement that to equip/attack method)
		case 4:
		{
			inv.manageInventory(p1, registry, true);
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
				sound1.play();
				cout << "Enter anything to proceed\n";
				cin >> anything;
				sound1.stop();
				refreshScreen();
			}
			//case where the enemy tries to rest
			else if (eRoll <= 9)
			{
				cout << enemy.getName() << " rests and restores some stamina!" << endl;
				enemy.rest();
				sound2.play();
				cout << "Enter anything to proceed\n";
				cin >> anything;
				sound2.stop();
				refreshScreen();
			}
			// case where enemy attempts to flee
			else {
				bool check = enemy.flee();
				if (check)
				{
					hasRun = true;
				}
				sound3.play();
				cout << "Enter anything to proceed";
				cin >> anything;
				sound3.stop();
				refreshScreen();
			}
		}
		// ends once a party dies or escapes
	} while ((p1.getHp() > 0 && enemy.getHp() > 0) && !hasRun);

	refreshScreen();

	Music.stop();

}
