#include "item.h"
#include <iostream>
#include <string>
#include <memory>
using std::string;
using std::cout;
using std::endl;


// item constructor
Item::Item(int id, const string& name, const string& description, int value, bool consumable, stats itemstats)
    : Entity(name, description), id(id), value(value), consumable(consumable) {
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

int Item::getId() const {
    return id;
}

// get item value
int Item::getValue() const {
    return value;
}

// see if edible lol
bool Item::isConsumable() const {
    return consumable;
}

stats Item::getStats()
{
    return itemStats;
}