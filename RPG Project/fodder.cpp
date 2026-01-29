#include "fodder.h"
fodder::fodder(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap) 
	: enemy(name, hp, melee, range, armor, stamina, dmg, ap) {

}