#include <iostream>
using namespace std;
void returnToOverworld() {

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
	}
		 



	return 0;
}