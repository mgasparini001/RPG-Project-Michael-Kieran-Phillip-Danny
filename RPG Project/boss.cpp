#include "boss.h"
#include <iostream>

boss::boss(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap)
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}

void boss:: attackMessage(Character& target)
{
	std::cout << Name << " strikes " << target.getName() << " with full force!";
}
