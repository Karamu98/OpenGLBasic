#include "glpch.h"
#include "Game.h"

int main()
{	
	Game* newGame = new Game();
	newGame->Run("Basic Lighting", 1920, 1080, false);

	delete newGame;

	return 0;
}
