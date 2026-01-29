#pragma once
#include "enemy.h"
class heavy :
    public enemy
{

    heavy(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap);
    
    void attackMessage(Character& target) override;



};

