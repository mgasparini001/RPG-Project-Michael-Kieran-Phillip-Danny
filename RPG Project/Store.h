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
	ItemRegistry stock;
public:
	//store class constructor, takes in a string representing a name and an itemregistry representing the main one for the game
	store(string n, ItemRegistry i);
	
	//Inputs: Player object pointer reprsenting the player who is buying something, Npc object representing the seller, and two ints represnting
	//        the id of the item and number of the item the player is buying
	//Description: Transfers the quanity of items the player wants to their inventory from the Npc's inventory, decreases necessary amount of gold
	//         from the player 
	void buySomething(player& buyer, npc& seller, int id, int quanity); 
	
	//Inputs: Player object pointer reprsenting the player who is selling something, Npc object pointer representing the buyer, and two ints represnting
	//        the id of the item and number of the item the player is selling
	//Description: Removes the number of necessary items from the players inventory and increaes the player's gold by the necessary amount
	void sellSomething(player& seller, npc& buyer, int id, int quanity);

	//Inputs: Player object pointer and Npc object pointer
	//Description: Simulates the store, allowing the buyer to buy from the Npc or sell to the Npc
	void enterStore(player& p, npc& o);



};
