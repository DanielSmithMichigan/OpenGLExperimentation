#include <iostream>
#include "Game.h"
#include "Model.h"
#include "CubeComponent.h"
#include "Grid.h"
#include <cstdio>
#include <chrono>
#include <thread>
#include <time.h>

using namespace std;

int main(void)
{
	srand(time(NULL));
	std::this_thread::sleep_for(std::chrono::seconds(3));
	Game* game = new Game();
	Model* model = new Model("Assets/sphere.dae");
	model->pushMeshesTo(game->components);
	//CubeComponent* cube = new CubeComponent();
	//game->components.push_back(cube);
	//Grid* grid = new Grid(10, 10);
	//game->components.push_back(grid);
	game->run();
	std::getchar();
}