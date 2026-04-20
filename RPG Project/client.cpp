#include <iostream>
#include "gameManager.h"

#ifdef _WIN32
#include <windows.h>
#endif

void refreshScreen()
{
	// clear screen (probably will be removed later).
	std::cout << std::string(60, '\n');
}

int main()
{
	try
	{
		GameManager game;

		if (!game.initialize())
		{
			std::cerr << "Failed to initialize game!" << std::endl;
			return -1;
		}

		game.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main();
}
#endif
