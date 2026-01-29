#pragma once
#include "character.h"
#include "entity.h"
#include "item.h"
#include <string>
using std::string;

// player class derived from Character
class player : public Character
{
private:
	int stamina = 1000;
	int XP = 3000;
	int gold = 100;
public:
	// gold and experience pointsz
	
	// player constructor
	player(const string& name, int HP, int melee, int range, int Armor, int strength);

	// override attack function
	void attack(Character& target, bool attackType) override;

	// interact with entity (npcs, items, etc)
	void interact(Entity& entity);


	// flee function
	bool flee();
};

