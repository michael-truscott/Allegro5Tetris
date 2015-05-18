#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include "TetrisGame.h"


int main()
{
	srand(time(NULL));
	TetrisGame game;
	while (!game.ShouldQuit())
	{
		game.Update();
		game.Render();
		al_rest(1.0f / FRAMES_PER_SEC);
	}

	return 0;
}