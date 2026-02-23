#include "player.h"
#include <iostream>
using std::cout;
using std::endl;

// player constructor
player::player(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap, int x, int g)
    : Character(name, hp, melee, range, armor, stamina, dmg, ap, false) {
    gold = g;
    XP = x;
}

//Input: Address of Character object, Output: void, prints a message that signifies the player's attack
void player::attackMessage(Character& target)
{
    cout << Name << " hits " << target.getName() << " for some damage." << endl;
}

// interact with entity (npcs, items, whatever else we add later)

void player::interact(Entity& entity) {
    cout << getName() << " interacts with " << entity.getName() << "." << endl;
    cout << entity.getDescription() << endl;
    
    // check if item and if so check if consumable, ask to use
    Item* item = dynamic_cast<Item*>(&entity);
    if (item && item->isConsumable()) {
        cout << "Use " << item->getName() << "? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            item->use(getName());
        } else {
            // do nothing
        }
    }
}

int player::getGold()
{
    return gold;
}

int player::getXp()
{
    return XP;
}

void player::setGold(int g)
{
    gold = g;
}

void player::setXp(int x)
{
    XP = x;
}




