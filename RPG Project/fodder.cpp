#include "fodder.h"
#include <iostream>

//fodder object constructor
fodder::fodder(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap) 
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}

//Input: Address of Character object, Output: void, prints a message that signifies a fodders' attack
void fodder::attackMessage(Character& target)
{
	std::cout << Name << " swings widly into " << target.getName() << "!" << std::endl;
}