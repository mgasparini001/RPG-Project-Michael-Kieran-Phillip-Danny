#include "enemy.h"
#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

// enemy constructor
enemy::enemy(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg, int XP)
    : Character(name, hp, move, dex, str, wits, toughness, dmg), XP(XP) {
}

// override attack function for enemy-specific behavior
void enemy::attack(Character& target) {
    cout << getName() << " attacks " << target.getName() << " for " << dmg << " damage!" << endl;
    target.takeDamage(dmg);
}

// loot drop, returns random amount of gold
int enemy::lootDrop() {
    int gold = rand() % 50 + 10; // random gold between 10 and 59
    cout << getName() << " dropped " << gold << " gold!" << endl;
    return gold;
}