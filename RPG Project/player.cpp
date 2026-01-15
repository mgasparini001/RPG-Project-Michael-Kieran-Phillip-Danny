#include "player.h"
#include <iostream>
using std::cout;
using std::endl;

player::player(const string& name, int hp, int move, int dex, int str, int wits, int dmg)
    : Character(name, hp, move, dex, str, wits, 0, dmg) {
}

void player::attack(Character& target) {
    cout << getName() << " attacks " << target.getName() << " for " << dmg << " damage!" << endl;
    target.takeDamage(dmg);
}

void player::interact() {
    cout << getName() << " interacts with the environment." << endl;

}


void player::flee() {
    cout << getName() << " attempts to flee from combat!" << endl;
}
