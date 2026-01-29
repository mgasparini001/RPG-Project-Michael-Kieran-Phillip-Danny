#include "player.h"
#include <iostream>
using std::cout;
using std::endl;

// player constructor
player::player(const string& name, int HP, int melee, int range, int Armor, int AP)
    : Character(name, HP, melee, range, Armor) {
}

// override attack function to include player-specific behavior
void player::attack(Character& target, bool attackType) {
    int damage = 0;

    // attackType
    if (attackType) {
        if (stamina > 100) {
            stamina -= 100;
            //melee attack
            if (diceRoll(melee) + AP > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for AP and dmg)
                target.takeDamage(diceRoll(melee) + dmg);
            }
        }
        else {
            cout << "You are too exhuasted to make the attack!" << endl;
        }
    }
    else {
        //ranged attack
        if (stamina > 50) {
            stamina -= 50;
            if (diceRoll(AP) > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for dmg, AP for AP)
                target.takeDamage(diceRoll(range));
            }
        }
        else {
            cout << "You are too exhuasted to make the attack!" << endl;
        }
    }
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
bool player::flee() {
    cout << getName() << " attempts to flee from combat!" << endl;
    
    // roll a 20 side dice to see if they succeed
    int fleeRoll = diceRoll(20);
    
    if (fleeRoll >= 15) {
        cout << getName() << " successfully escaped!" << endl;
        return true;
    } else {
        cout << getName() << " failed to escape!" << endl;
        return false;
    }
}