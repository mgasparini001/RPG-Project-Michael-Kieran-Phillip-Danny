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
	int XP;
	int gold;
public:
	
	
	// player constructor
	player(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap, int x, int g);

	//overridden attackMessage function
	void attackMessage(Character& target) override;

	// interact with entity (npcs, items, etc)
	void interact(Entity& entity);

	int getGold();

	int getXp();

	void setGold(int g);

	void setXp(int xp);
};

