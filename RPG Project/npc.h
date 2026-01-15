#pragma once
class npc
{
public:
	int HP;
	int T;
	int Move;
	int Dex;
	int Str;
	int XPdrop;

	npc(int hp, int t, int move, int dex, int str, int xpdrop) {
		HP = hp;
		T = t;
		Move = move;
		Dex = dex;
		Str = str;
		XPdrop = xpdrop;
	}
	//loot drop
};

