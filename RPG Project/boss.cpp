#include "boss.h"
#include <iostream>

//boss object constructor
boss::boss(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap)
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}

//Input: Address of Character object, Output: void, prints a message that signifies a bosses' attack 
void boss:: attackMessage(Character& target)
{
	std::cout << Name << " strikes " << target.getName() << " with full force!" << std::endl;
}
