#pragma once
#include "character.h"
#include <string>
using std::string;

// enemy class derived from Character
class enemy :public Character
{
public:
	// enemy constructor
	enemy(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap);



	// loot drop function
	int lootDrop();

};

