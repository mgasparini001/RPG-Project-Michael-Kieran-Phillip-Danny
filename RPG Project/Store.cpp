#include "ItemRegistry.h"
#include "Inventory.h"
#include <iostream>
#include <string>
#include "npc.h"
#include "Store.h"
#include "player.h"

store::store(string n, ItemRegistry i)
{
	storeName = n;
	stock = i;
}

void store :: buySomething(player& buyer, npc& seller, int id, int quanity)
{
	if (seller.hasItem(id))
	{
		if (quanity <= seller.getInventory().getQuantity(id))
		{
			if (buyer.getGold() >= (seller.getInventory().getItem(id)->getValue() * quanity))
			{
				buyer.setGold((buyer.getGold() - (seller.getInventory().getItem(id)->getValue() * quanity)));
				seller.removeItemFromInventory(id, seller, quanity);
				buyer.addItemToInventory(id, stock, quanity);
				std::cout << "Thanks for doing business" << std::endl;
			}
			else
			{
				std::cout << "You do not have enough gold to get that much, maybe get a job or something" << std::endl;
			}
		}
		else
		{
			std::cout << "I do not have that much bro, the max number I have is listed im my inventory" << std::endl;
		}
	}
	else
	{
		std::cout << "Is " << stock.getItemName(id) << " in the room with us?" << std::endl;
	}
}

void store::sellSomething(player& seller, npc& buyer, int id, int quanity)
{
	if (seller.hasItem(id))
	{
		if (quanity <= seller.getInventory().getQuantity(id))
		{
			seller.setGold((seller.getGold()) + (seller.getInventory().getItem(id)->getValue() * quanity));
			seller.removeItemFromInventory(id, seller, quanity);
			std::cout << "Thanks for doing business" << std::endl;
		}
		else
		{
			std::cout << "I would buy that much if you actually had that much bro" << std::endl;
		}
	}
	else
	{
		std::cout << "Why are you trying to sell me something you do not have?" << std::endl;
	}
}

void store::enterStore(player& p, npc& o)
{
	int num;
	std::cout << "1. Buy\n2. Sell" << std::endl;
	std::cin >> num;

	if (num == 1)
	{
		o.getInventory().printInventory();
	}
}