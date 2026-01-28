#include "enemy.h"
#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

// enemy constructor
enemy::enemy(const string& name, int HP, int melee, int range, int Armor, int XP)
    : Character(name, HP, melee, range, Armor), XP(XP) {
}

// override attack function for enemy-specific behavior
void enemy::attack(Character& target, bool attackType) {
    int damage = 0;
    
    // attackType
    if (attackType) {
        //melee attack
    } else {
        //ranged attack
    }
    
    cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!" << endl;
    // Apply damage to target
    target.takeDamage(damage);
}

// loot drop, returns random amount of gold
int enemy::lootDrop() {
    int gold = diceRoll(50) + 10; // random gold between 10 and 59
    cout << getName() << " dropped " << gold << " gold!" << endl;
    return gold;
}