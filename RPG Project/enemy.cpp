#include "enemy.h"
#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

// enemy constructor
enemy::enemy(const string& name, int HP, int melee, int range, int Armor, int XP, int strength)
    : Character(name, HP, melee, range, Armor), XP(XP) {
}

// override attack function for enemy-specific behavior
void enemy::attackMessage(){};

void rest() {}

// loot drop, returns random amount of gold
int enemy::lootDrop() {
    int gold = diceRoll(50) + 10; // random gold between 10 and 59
    cout << getName() << " dropped " << gold << " gold!" << endl;
    return gold;
}