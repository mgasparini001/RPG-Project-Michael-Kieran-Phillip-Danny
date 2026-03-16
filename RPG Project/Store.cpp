#include "Store.h"



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
	bool valid = false;
	while (!valid)
	{
		string test = getValidInput();
		if (test != "fail")
		{
			num = stoi(test);
			if (num < 1 || num > 3)
			{
				std::cout << "Invalid input. Please enter one of the numbers on screen" << std::endl;
			}
			else
			{
				valid = true;
			}
		}
	}

	while (num != 3)
	{
		if (num == 1)
		{
			refreshScreen();
			int i;
			int q;
			o.getInventory().printInventoryStore(o, p);
			std::cout << o.getName() << ": What item would you like to buy " << p.getName() << "?\n";
			bool valid = false;
			while (!valid)
			{
				string test = getValidInput();
				if (test != "fail")
				{
					i = stoi(test);
					valid = true;
				}
			}
			if (o.hasItem(i))
			{
				std::cout << "How much do you want?" << std::endl;
				bool valid = false;
				while (!valid)
				{
					string test = getValidInput();
					if (test != "fail")
					{
						q = stoi(test);
						valid = true;
					}
				}
				if (q <= o.getInventory().getQuantity(i))
				{
					int price = (o.getInventory().getItem(i)->getValue() * q);
					std::cout << "That will be " << price << " gold" << std::endl;
					if(p.getGold() >= price)
					{
						string response;
						std::cout << "Are you sure? (enter yes to go through with your purchase): ";
						std::cin >> response;
						if (response != "yes")
						{
							std::cout << "No deal I guess then, these items may be important you know" << std::endl;
							std::cout << "Press enter to proceed";
							std::cin.ignore();
							std::cin.get();
							refreshScreen();
						}
						else
						{
							buySomething(p, o, i, q);
						}
					}
					else
					{
						
						std::cout << "You do not have enough gold, maybe go get a job or something";
						std::cout << "Press enter to proceed";
						std::cin.ignore();
						std::cin.get();
						refreshScreen();
					}
				}
				else
				{
					std::cout << "I do not have that much bro, the max number I have is listed in front of your face!" << std::endl;
					std::cout << "Press enter to proceed";
					std::cin.ignore();
					std::cin.get();
					refreshScreen();
				}
			}
			else
			{
				std::cout << "Is this item in the room with us? Am I going insane?" << std::endl;
				std::cout << "Press enter to proceed";
				std::cin.ignore();
				std::cin.get();
				refreshScreen();
			}
		}
		else
		{
			refreshScreen();
			int i;
			int q;
			p.getInventory().printInventoryStore(o, p);
			std::cout << o.getName() << ": Whatcha got for me, " << p.getName() << "?\n";
			bool valid = false;
			while (!valid)
			{
				string test = getValidInput();
				if (test != "fail")
				{
					i = stoi(test);
					valid = true;
				}
			}
			if (p.hasItem(i))
			{
				std::cout << "How much do you want to sell?";
				bool valid = false;
				while (!valid)
				{
					string test = getValidInput();
					if (test != "fail")
					{
						q = stoi(test);
						valid = true;
					}
				}
				if (q <= p.getInventory().getQuantity(i))
				{
					int price = p.getInventory().getItem(i)->getValue() * q;
					std::cout << "I can give you " << price << " gold for that" << std::endl;
					string response;
					std::cout << "Are you sure? (enter yes to continue with your selling): ";
					std::cin >> response;
					if (response != "yes")
					{
						std::cout << "No deal I guess then, the gold may be important you know" << std::endl;
						std::cout << "Press enter to proceed";
						std::cin.ignore();
						std::cin.get();
						refreshScreen();
					}
					else
					{
						sellSomething(p, o, i, q);
					}
				}
				else
				{
					std::cout << "I would buy that much if you actually had that much bro" << std::endl;
					std::cout << "Press enter to proceed";
					std::cin.ignore();
					std::cin.get();
					refreshScreen();
				}
			}
			else
			{
				std::cout << "Why are you trying to sell me something you do not have? What do you take me for?" << std::endl;
				std::cout << "Press enter to proceed";
				std::cin.ignore();
				std::cin.get();
				refreshScreen();
			}
		}
		
		std::cout << "\n1. Buy\n2. Sell\n3. Exit" << std::endl;
		bool valid = false;
		while (!valid)
		{
			string test = getValidInput();
			if (test != "fail")
			{
				num = stoi(test);
				if (num < 1 || num > 3)
				{
					std::cout << "Invalid input. Please enter one of the numbers on screen" << std::endl;
				}
				else
				{
					valid = true;
				}
			}
		}
	}
	refreshScreen();
}