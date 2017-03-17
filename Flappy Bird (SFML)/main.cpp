#include <cstdio>
#include <ctime>
#include <iostream>
#include "Game.h"

int main() 
{
	std::srand(time(NULL));
	try
	{
		Game game;
		game.Run();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what();
	}
	return 0;
}