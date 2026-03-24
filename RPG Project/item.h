#pragma once
#include "entity.h"
#include <string>
#include "stats.h"

// item class for anything in inventory, inherits from entities
class Item : public Entity
{
protected:
    int id;
    int value; // prob in gold
    bool consumable; // whether its consumable (apple or potion or smthn)
    stats itemStats;

public:
    
    // item constructor
    Item(int id, const std::string& name, const std::string& description, int value, bool consumable, stats itemstats);

    // virtual destructor (bc stuff will prob need to derive from Item)
    virtual ~Item() = default;

    // use item (this is also virtual so derived stuff can do specific things)
    virtual void use(const std::string& playerName = "");

    // get item id
    int getId() const;

    // get item value
    int getValue() const;

    // check if player can consume
    bool isConsumable() const;

    virtual stats getStats();

    
};
