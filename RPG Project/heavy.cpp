#include "heavy.h"
#include <iostream>

//heavy object constructor
heavy::heavy(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap)
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}

//Input: Address of Character object, Output: void, prints a message that signifies a heavy's attack
void heavy::attackMessage(Character& target)
{
	std::cout << Name << " delivers a crushing blow to " << target.getName() << "!" << std::endl;
}