#include "item.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;

// item constructor
Item::Item(const string& name, const string& description, int value, bool consumable)
    : Entity(name, description), value(value), consumable(consumable) {
}

// use item
void Item::use(const string& playerName) {
    if (!playerName.empty()) {
        cout << playerName << " uses " << name << "." << endl;
    } else {
        cout << "You use " << name << "." << endl;
    }
    if (consumable) {
        cout << name << " has been consumed." << endl;
    }
}

// get item value
int Item::getValue() const {
    return value;
}

// see if edible lol
bool Item::isConsumable() const {
    return consumable;
}
