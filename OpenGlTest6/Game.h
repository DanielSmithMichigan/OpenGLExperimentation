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
#include "ProgramHandle.h"

using namespace std;

class Game {
	private:
		void init();
		void loadShaders();
		void linkProgram();
		void initGlew();
		void initializeComponents();
		GLuint getProgramHandle();
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
	window = new Window(800, 600, "Ya did it harry");
	initGlew();
	getProgramHandle();
	loadShaders();
}

void Game::run() {
	linkProgram();
	initializeComponents();
	window->render(components);
	window->destroy();
}

void Game::initializeComponents() {
	for (Component* component : components)
	{
		component->Initialize();
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

void Game::loadShaders() {
	GLuint programHandle = ProgramHandle::getProgramHandle();
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "shader.vert", programHandle);
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, "shader.frag", programHandle);
	glBindAttribLocation(programHandle, 0, "VertexPosition");
	glBindAttribLocation(programHandle, 1, "VertexColor");
}

GLuint Game::getProgramHandle() {
	GLuint pHandle = glCreateProgram();
	if (0 == pHandle)
	{
		Error::showError("Error creating program object.\n", true);
		return NULL;
	} else {
		ProgramHandle::setProgramHandle(pHandle);
		return pHandle;
	}
}

void Game::linkProgram() {
	GLuint programHandle = ProgramHandle::getProgramHandle();
	glLinkProgram(programHandle);

	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		Error::showError("Failed to link shader program!\n", false);
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			Error::showError("Program log: \n%s", true);
			free(log);
		}
	}
	else
	{
		glUseProgram(programHandle);
	}
}