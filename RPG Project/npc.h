#pragma once
#include "character.h"
#include "entity.h"
#include <string>
using std::string;

// npc class derived from both character and entity 
// bc that way they can both have stats and be interacted with

class npc : public Character, public Entity
{
public:
	int xpDrop;

	// npc constructor
	npc(const string& name, const string& description, int HP, int melee, int range, int Armor, int stamina, int damage, int ap, bool hasItem, int xp);

	// interact function
	void interact();

	// loot drop funct
	int lootDrop() const;

	void attackMessage(Character& target) override;

};

