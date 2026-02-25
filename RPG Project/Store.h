#pragma once
#include "ItemRegistry.h"
#include "Inventory.h"
#include <iostream>
#include <string>
#include "npc.h"
class store
{
private:
	npc* owner; //inventory given in client?
	string storeName;
	ItemRegistry* stock; //just pass in the one itemregistry, just so it can be used in the store class without any shenanigans
public:
	store(npc* o, string n, ItemRegistry* i);
	
	void buySomething(player& buyer, int id, int quanity); //checks if npc has quanity and if player has enough gold, if so transfer items by
	                                                       //simply changing quanities that way we only need one of each object

	void sellSomething(player& seller, int id, int quanity); //checks if player has enough to sell, if so give gold and remove necessary amount

	string getName(); //might not be necessary

	npc* getOwner(); //might not be necessary

	void printStore(); //might be useful to have this in here to reduce size of client

};
