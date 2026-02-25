#pragma once
#include "ItemRegistry.h"
#include "Inventory.h"
#include <iostream>
#include <string>
#include "npc.h"
class store
{
private:
	string storeName;
	ItemRegistry stock; //just pass in the one itemregistry, just so it can be used in the store class without any shenanigans
public:
	store(string n, ItemRegistry i);
	
	void buySomething(player& buyer, npc& seller, int id, int quanity); //checks if npc has quanity and if player has enough gold, if so transfer items by
	                                                       //simply changing quanities that way we only need one of each object
	void sellSomething(player& seller, npc& buyer, int id, int quanity); //checks if player has enough to sell, if so give gold and remove necessary amount

	void enterStore(player& p, npc& o); //we can put the whole store function in this method to reduce size of client

	void printStore(); //might be useful to have this in here to reduce size of client

};
