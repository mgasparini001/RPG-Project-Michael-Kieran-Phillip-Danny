#pragma once
#include "character.h"
#include "entity.h"
#include "item.h"
#include <string>
using std::string;

// player class derived from Character
class player : public Character
{
public:
	// gold and experience pointsz
	int XP = 3000;
	int gold = 100;

	// player constructor
	player(const string& name, int HP, int melee, int range, int Armor, int strength);

	// override attack function
	void attack(Character& target, bool attackType) override;

	// interact with entity (npcs, items, etc)
	void interact(Entity& entity);


	// flee function
	bool flee();

	int getStr();

	int getArmor();

	int getDmg();
};

