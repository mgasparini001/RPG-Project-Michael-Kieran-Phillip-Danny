#include "heavy.h"
#include <iostream>

heavy::heavy(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap)
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}

void heavy::attackMessage(Character& target)
{
	std::cout << Name << " delivers a crushing blow to " << target.getName() << "!" << std::endl;
}