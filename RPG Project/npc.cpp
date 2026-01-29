#include "npc.h"
#include <iostream>
using std::cout;
using std::endl;

// npc constructor
npc::npc(const string& name, const string& description, int HP, int melee, int range, int Armor, int xpDrop)
    : Character(name, HP, melee, range, Armor), Entity(name, description), xpDrop(xpDrop) {}

// interact with player
void npc::interact() {
    cout << Character::getName() << ": " << Entity::getDescription() << endl;
}

// loot drop for xp
int npc::lootDrop() const {
    return xpDrop;
}
