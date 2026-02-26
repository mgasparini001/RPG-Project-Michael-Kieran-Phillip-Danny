#include "ItemRegistry.h"
#include "Inventory.h"
#include <iostream>
#include <string>
#include "npc.h"
#include "Store.h"
#include "player.h"

//forward declare
void refreshScreen();

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
	refreshScreen();
	std::cout << "Hehe, thank you!" << std::endl;
}

void store::sellSomething(player& seller, npc& buyer, int id, int quanity)
{
	seller.setGold((seller.getGold()) + (seller.getInventory().getItem(id)->getValue() * quanity));
	seller.removeItemFromInventory(id, seller, quanity);
	refreshScreen();
	std::cout << "Hehe, thank you!" << std::endl;
}

void store::enterStore(player& p, npc& o)
{
	refreshScreen();
	int num;
	std::cout << o.getName() << ": " << p.getName() << "! It's been a while, how can I help you?\n";
	std::cout << p.getName() << ": Hey " << o.getName() << std::endl;
	std::cout << "\n1. Buy\n2. Sell\n3. Exit" << std::endl;
	std::cin >> num;

	while (num != 3)
	{
		if (num == 1)
		{
			refreshScreen();
			int i;
			int q;
			o.getInventory().printInventoryStore(o, p);
			std::cout << o.getName() << ": What item would you like to buy " << p.getName() << "?\n";
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
						std::cout << "Are you sure? (enter 1 to quit): ";
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
					std::cout << "I do not have that much bro, the max number I have is listed in front of your face!" << std::endl;
				}
			}
			else
			{
				std::cout << "Is this item in the room with us? Am I going insane?" << std::endl;
			}
		}
		else
		{
			int i;
			int q;
			p.getInventory().printInventoryStore(o, p);
			std::cout << o.getName() << ": Whatcha got for me, " << p.getName() << "?\n";
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
					std::cout << "Are you sure? (enter 1 to quit): ";
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
				std::cout << "Why are you trying to sell me something you do not have? What do you take me for?" << std::endl;
			}
		}
		
		std::cout << "\n1. Buy\n2. Sell\n3. Exit" << std::endl;
		std::cin >> num;
		
	}
	refreshScreen();
}