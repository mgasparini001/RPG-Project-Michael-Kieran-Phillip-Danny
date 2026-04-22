#include "npc.h"
#include "ItemRegistry.h"
#include "Bartender.h"
#include "GoblinKing.h"
#include <iostream>
#include "player.h"
using namespace std;

void chat(npc npc1, npc npc2, player& p1, ItemRegistry& registry, Inventory Inv)
{
	int choice;
	cout << "Talk with:\n";
	cout << R"(
-------------------------------
| 1. )"; cout << npc1.getName(); cout << R"(                |
| 2. )"; cout << npc2.getName(); cout << R"(          |
|                             |
-------------------------------
)";
	bartender guy1(9);
	goblinKing guy2(12);
	boss goblin("Goblin King", 50, 5, 4, 10, 1000, 9, 6);
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
		guy1.printDialouge(0);
	}
	else
	{
		guy2.printDialogueG(0, p1, goblin, registry, Inv);
	}

}