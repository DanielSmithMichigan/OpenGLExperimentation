#include <iostream>
#include "Game.h"
#include "CubeComponent.h"
#include <cstdio>
#include <chrono>
#include <thread>

using namespace std;

int main(void)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	Game* game = new Game();
	CubeComponent* component = new CubeComponent();
	game->addComponent(component);
	game->run();
	std::getchar();
}