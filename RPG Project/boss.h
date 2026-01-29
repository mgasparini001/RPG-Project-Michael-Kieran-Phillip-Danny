#pragma once
#include "enemy.h"
class boss :
    public enemy
{
public:
    boss(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap);
    void attackMessage() override;





};

