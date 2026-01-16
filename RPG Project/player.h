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
	// gold and experience points
	int XP = 3000;
	int gold = 100;

	// player constructor
	player(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg);

	// override attack function
	void attack(Character& target) override;

	// interact with entity (npcs, items, etc)
	void interact(Entity& entity);

	// flee function
	void flee() {}
};

