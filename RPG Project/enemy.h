#pragma once
#include "character.h"
#include <string>
using std::string;

// enemy class derived from Character
class enemy :public Character
{
public:
	// experience points given when defeated
	int XP;

	// enemy constructor
	enemy(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg, int XP);

	// override attack function for enemy-specific behavior
	void attack(Character& target) override;

	// loot drop function
	int lootDrop();

};

