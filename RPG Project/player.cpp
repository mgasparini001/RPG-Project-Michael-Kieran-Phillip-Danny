#include "player.h"
#include <iostream>
using std::cout;
using std::endl;

// player constructor
player::player(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg)
    : Character(name, hp, move, dex, str, wits, toughness, dmg) {
}

// override attack function to include player-specific behavior
void player::attack(Character& target) {
    cout << getName() << " attacks " << target.getName() << " for " << dmg << " damage!" << endl;
    target.takeDamage(dmg);
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

// flee function
void player::flee() {
    cout << getName() << " attempts to flee from combat!" << endl;
    
    // roll a 20 side dice plus dex to see if they succeed
    int fleeRoll = diceRoll(20) + dex;
    
    if (fleeRoll >= 15) {
        cout << getName() << " successfully escaped!" << endl;
    } else {
        cout << getName() << " failed to escape!" << endl;
    }
}
