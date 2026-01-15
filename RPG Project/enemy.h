#pragma once
#include "character.h"
#include <string>
using std::string;

class enemy :public Character
{
public:

	enemy(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg);

	void attack(Character& target) override;

	int lootDrop();

};

