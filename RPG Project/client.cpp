#include <iostream>
using namespace std;
void returnToOverworld() {
	cout << "returned to overworld";
}

void enterBattle() {
	cout << "entered battle";
}

void manageInventory() {
	cout << "managed inventory";
}

void enterShop() {
	cout << "entered shop";
}

void chat() {
	cout << "Talked to someone";
}

int main() {
	cout << R"(MENU:

1. Return to Game
2. Enter Battle
3. Manage Inventory
4. Enter Shop
5. Talk to Someone
6. Quit Game

		)";
	int num;
	cin >> num;
	switch (num) {
	case 1:
		returnToOverworld();
	
	case 2:
		enterBattle();
	}
		 



	return 0;
}