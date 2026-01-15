#pragma once
#include "character.h"
class player :public Character
{
	
	
	public:
		int XP = 3000;
		int gold = 100;

	player(const string& name, int hp, int stamina, int move, int dex, int str, int wits, int dmg);

	void attack(Character& target) override;

	void interact() {}

		void flee() {}

	
};

