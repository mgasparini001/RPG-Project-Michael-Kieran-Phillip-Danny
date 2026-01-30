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
	int XP = 3000;
	int gold = 100;
public:
	
	
	// player constructor
	player(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap);

	//overridden attackMessage function
	void attackMessage(Character& target) override;

	// interact with entity (npcs, items, etc)
	void interact(Entity& entity);
};

