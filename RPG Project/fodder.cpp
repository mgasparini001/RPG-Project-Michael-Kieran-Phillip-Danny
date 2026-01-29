#include "fodder.h"
#include <iostream>

fodder::fodder(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap) 
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}

void fodder::attackMessage(Character& target)
{
	std::cout << Name << " swings widly into " << target.getName() << "!" << std::endl;
}