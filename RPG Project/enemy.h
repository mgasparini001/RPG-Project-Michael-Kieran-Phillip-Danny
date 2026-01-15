#pragma once
#include "character.h"
class enemy :public character
{
public:
	int XPdrop;

	enemy(int hp, int t, int move, int dex, int str, int xpdrop, int dmg) {
		HP = hp;
		T = t;
		Move = move;
		Dex = dex;
		Str = str;
		XPdrop = xpdrop;
		Dmg = dmg;
	}
	//loot drop
};

