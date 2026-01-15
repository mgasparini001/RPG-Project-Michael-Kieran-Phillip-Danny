#include <iostream>
#include <string>
#include <cstdlib>
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
string quitGame(string &menu) {
	return menu;
}

//rolls a dice based on an input (x = 6 would mean a six sided die
int diceRoll(int x)
{
	return (rand() % x) + 1;
}

int main() {

	srand(static_cast<unsigned int>(time(0)));
	cout << diceRoll(6);
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
		quitGame(menu);
		break;




	}

	cout << "hello";
		 



	return 0;
}