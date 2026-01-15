#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
using namespace std;

void returnToOverworld() {
	cout << "returned to overworld\n";

}

void enterBattle() {
	cout << "entered battle\n";
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

int main() {

	srand(static_cast<unsigned int>(time(0)));
	cout << diceRoll(6);

	
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