#pragma once
#include "ItemRegistry.h"
#include "Inventory.h"
#include <iostream>
#include <string>
#include "npc.h"
class store
{
private:
	npc* owner;
	string storeName;
public:
	store(npc* o, string n);
	
	void buySomething(player& buyer);

	void sellSomething(player& seller);

};
