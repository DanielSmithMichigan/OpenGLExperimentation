#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Error.h"
#include "Component.h"
#include "Window.h"
#include <vector>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Game {
	private:
		void init();
		void loadShaders();
		void linkProgram();
		void initGlew();
		void initializeComponents();
		int GAME_WIDTH = 800;
		int GAME_HEIGHT = 600;
	public:
		void run();
		void addComponent(Component* component);
		vector<Component*> components;
		Window* window;
		Game();
};

Game::Game() {
	init();
}

void Game::addComponent(Component* component) {
	components.push_back(component);
}

void Game::init() {
	GlobalGameObjects *objects = new GlobalGameObjects(GAME_WIDTH, GAME_HEIGHT);
	window = new Window(GAME_WIDTH, GAME_HEIGHT, "Ya did it harry", objects);
	initGlew();
}

void Game::run() {
	initializeComponents();
	window->render(components);
	window->destroy();
}

void Game::initializeComponents() {
	for (Component* component : components)
	{
		component->doInitialize();
	}
}

void Game::initGlew() {
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}
}