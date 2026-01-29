#pragma once
#include "character.h"
#include <string>
using std::string;

// *** Possibly need to make inherited classes from enemy class for "enemy types" ***

// enemy class derived from Character
class enemy :public Character
{
public:
	// experience points given when defeated
	int XP;

	// enemy constructor
	enemy(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap);
	
	//manages attack message. overridden later
	virtual void attackMessage()=0;


	// loot drop function
	int lootDrop();

	virtual void rest()=0;
};

