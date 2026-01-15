#pragma once
#include "character.h"
#include <string>
using std::string;

class enemy :public Character
{
public:
	int XPdrop;

	enemy(const string& name, int hp, int stamina, int move, int dex, int str, int wits, int dmg, int xpdrop);

	void attack(Character& target) override;

	int lootDrop();

	
};

