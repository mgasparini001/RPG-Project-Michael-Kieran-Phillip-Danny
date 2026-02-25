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
	buyer.setGold((buyer.getGold() - (seller.getInventory().getItem(id)->getValue() * quanity)));
	seller.removeItemFromInventory(id, seller, quanity);
	buyer.addItemToInventory(id, stock, quanity);
	std::cout << "Thanks for doing business" << std::endl;
}

void store::sellSomething(player& seller, npc& buyer, int id, int quanity)
{
	seller.setGold((seller.getGold()) + (seller.getInventory().getItem(id)->getValue() * quanity));
	seller.removeItemFromInventory(id, seller, quanity);
	std::cout << "Thanks for doing business" << std::endl;
}

void store::enterStore(player& p, npc& o)
{
	int num;
	std::cout << "1. Buy\n2. Sell\n3. Exit" << std::endl;
	std::cin >> num;
	while (num != 3)
	{
		if (num == 1)
		{
			int i;
			int q;
			o.getInventory().printInventoryStore(o);
			std::cout << "What item would you like to buy " << p.getName() << "? You have " << p.getGold() << std::endl;
			std::cin >> i;
			std::cout << "How much do you want?: ";
			std::cin >> q;
			if (o.hasItem(i))
			{
				if (q <= o.getInventory().getQuantity(i))
				{
					int price = (o.getInventory().getItem(i)->getValue() * q);
					std::cout << "That will be " << price << " gold" << std::endl;
					if(p.getGold() >= price)
					{
						int c;
						std::cout << "Are you sure?(enter 1 if you want to back out): ";
						std::cin >> c;
						if (c == 1)
						{
							std::cout << "No deal I guess then, these items may be important you know" << std::endl;
						}
						else
						{
							buySomething(p, o, i, q);
						}
					}
					else
					{
						std::cout << "You do not have enough gold, maybe go get a job or something";
					}
				}
				else
				{
					std::cout << "I do not have that much bro, the max number I have is listed im my store" << std::endl;
				}
			}
			else
			{
				std::cout << "Is this item in the room with us?" << std::endl;
			}
		}
		else
		{
			int i;
			int q;
			p.getInventory().printInventoryStore(p);
			std::cout << "What item would you like to sell " << p.getName() << "? You have " << p.getGold() << " Gold";
			std::cin >> i;
			std::cout << "How much do you want to sell?: ";
			std::cin >> q;
			if (p.hasItem(i))
			{
				if (q <= p.getInventory().getQuantity(i))
				{
					int price = p.getInventory().getItem(i)->getValue() * q;
					std::cout << "I can give you " << price << " gold for that" << std::endl;
					int c;
					std::cout << "Are you sure?(enter 1 if you want to back out): ";
					std::cin >> c;
					if (c == 1)
					{
						std::cout << "No deal I guess then, the gold may be important you know" << std::endl;
					}
					else
					{
						sellSomething(p, o, i, q);
					}
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
		std::cout << "1. Buy\n2. Sell\n3. Exit" << std::endl;
		std::cin >> num;
	}
}