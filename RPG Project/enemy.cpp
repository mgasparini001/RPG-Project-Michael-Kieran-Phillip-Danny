#include "enemy.h"
#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

// enemy constructor
enemy::enemy(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap)
    : Character(name, hp, melee, range, armor, stamina, dmg, ap) {
}



//Output: int, loot drop, returns random amount of gold
int enemy::lootDrop() {
    int gold = diceRoll(50) + 10; // random gold between 10 and 59
    cout << getName() << " dropped " << gold << " gold!" << endl;
    return gold;
}