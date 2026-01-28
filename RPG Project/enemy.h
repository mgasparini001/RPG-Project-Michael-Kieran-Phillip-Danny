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
	enemy(const string& name, int HP, int melee, int range, int Armor, int XP);

	// override attack function for enemy-specific behavior
	void attack(Character& target, bool attackType) override;

	// loot drop function
	int lootDrop();

};

