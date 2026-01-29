#pragma once
#include "item.h"
#include <string>

// item class for anything in inventory, inherits from entities
class weapon : public Item
{
protected:
    bool isRanged; //ranged or not
    int AP;
    int dmg;


public:
    
};
