#include "npc.h"
#include <iostream>
using std::cout;
using std::endl;

// npc constructor
npc::npc(const string& name, const string& description, int hp, int move, int dex, int str, int wits, int toughness, int dmg, int xpDrop)
    : Character(name, hp, move, dex, str, wits, toughness, dmg), Entity(name, description), xpDrop(xpDrop) {}

// interact with player
void npc::interact() {
    cout << Character::getName() << ": " << Entity::getDescription() << endl;
}

// loot drop for xp
int npc::lootDrop() const {
    return xpDrop;
}
