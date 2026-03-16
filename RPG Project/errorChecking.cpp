#include "errorChecking.h"

std::string getValidInput()
{
	std::string input = " ";
	std::cin >> input;
	for (char c : input)
	{
		if (!isdigit(c))
		{
			std::cout << "Invalid input, please enter a number with no decimals" << std::endl;
			return "fail";
		}
	}
	return input;

}