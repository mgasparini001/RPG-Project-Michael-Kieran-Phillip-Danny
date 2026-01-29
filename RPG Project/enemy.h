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
	enemy(const string& name, int HP, int melee, int range, int Armor, int XP, int strength);
	
	//manages attack message. overridden later
	void virtual attackMessage();


	// loot drop function
	int lootDrop();

};

