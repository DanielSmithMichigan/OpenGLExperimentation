#include <iostream>
#include "Game.h"
#include "Model.h"
#include "CubeComponent.h"
#include <cstdio>
#include <chrono>
#include <thread>

using namespace std;

int main(void)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	Game* game = new Game();
	Model* model = new Model("Assets/sphere.dae");
	model->pushMeshesTo(game->components);
	game->run();
	std::getchar();
}