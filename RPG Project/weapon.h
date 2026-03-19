#pragma once
#include "item.h"
#include <string>

// item class for anything in inventory, inherits from entities
class weapon : public Item
{
public:
    bool isRanged; //ranged or not
    int AP;
    int dmg;

    weapon(int id, const std::string& name, const std::string& description, bool consumable, int value, bool IsRanged, int Dmg, int ap);
};
