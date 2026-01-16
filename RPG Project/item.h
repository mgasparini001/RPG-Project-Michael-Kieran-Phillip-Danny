#pragma once
#include "entity.h"
#include <string>

// item class for anything in inventory, inherits from entities
class Item : public Entity
{
protected:
    int value; // prob in gold
    bool consumable; // whether its consumable (apple or potion or smthn)

public:
    // item constructor
    Item(const std::string& name, const std::string& description, int value, bool consumable);

    // virtual destructor (bc stuff will prob need to derive from Item)
    virtual ~Item() = default;

    // use item (this is also virtual so derived stuff can do specific things)
    virtual void use(const std::string& playerName = "");

    // get item name
    std::string getName() const;

    // get item description
    std::string getDescription() const;

    // get item value
    int getValue() const;

    // check if player can consume
    bool isConsumable() const;
};
