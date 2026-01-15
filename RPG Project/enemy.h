#pragma once
#include "character.h"
#include <string>
using std::string;

class enemy :public Character
{
public:
	int XP;

	enemy(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg, int XP);

	void attack(Character& target) override;

	int lootDrop();

};

