#include "npc.h"
#include <iostream>
using std::cout;
using std::endl;

// npc constructor
npc::npc(const string& name, const string& description, int HP, int melee, int range, int Armor, int stamina, int damage, int ap, bool hasItem, int xpDrop)
   : Character(name, HP, melee, range, Armor, stamina, damage, ap, hasItem), Entity(name, description), xpDrop(xpDrop) {}

// interact with player
void npc::interact() {
    cout << Character::getName() << ": " << Entity::getDescription() << endl;
}

// loot drop for xp
int npc::lootDrop() const {
    return xpDrop;
}









void npc::attackMessage(Character& target)
{

}
