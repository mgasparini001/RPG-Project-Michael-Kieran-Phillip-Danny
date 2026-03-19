#pragma once
#include "weapon.h"
#include <string>

    weapon::weapon(int id, const std::string& name, const std::string& description, bool consumable, int value, bool isRanged, int dmg, int AP)
        : Item(id, name, description, value, consumable), isRanged(isRanged), dmg(dmg), AP(AP) {}