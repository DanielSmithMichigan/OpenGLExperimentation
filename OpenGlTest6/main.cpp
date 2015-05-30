#include <iostream>
#include "Game.h"
#include "TriangleComponent.h"
#include <cstdio>

using namespace std;

int main(void)
{
	Game* game = new Game();
	TriangleComponent* component = new TriangleComponent();
	game->addComponent(component);
	game->run();
	std::getchar();
}