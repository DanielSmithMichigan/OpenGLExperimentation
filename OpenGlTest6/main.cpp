#include <iostream>
#include "Game.h"
#include "CubeComponent.h"
#include <cstdio>

using namespace std;

int main(void)
{
	Game* game = new Game();
	CubeComponent* component = new CubeComponent();
	game->addComponent(component);
	game->run();
	std::getchar();
}