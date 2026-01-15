#pragma once
#include "character.h"
class player :public character
{
	class character
	{
	public:
		int XP = 3000;
		int gold = 100;

		void attack() {}

		void interact() {}

		void flee() {}

	};
};

