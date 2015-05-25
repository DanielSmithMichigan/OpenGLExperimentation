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
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Game {
	private:
		void loadShaders();
		void linkProgram();
		void initGlew();
		GLuint getProgramHandle();
		GLuint programHandle;
	public:
		Game();
};

Game::Game() {
	Window* myWindow = new Window(800, 600, "Ya did it harry");
	initGlew();
	getProgramHandle();
	loadShaders();
	Component* components[1] = { new Component(this->programHandle) };
	linkProgram();
	myWindow->render(components, 1);
	myWindow->destroy();
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
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "shader.vert", this->programHandle);
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, "shader.frag", this->programHandle);
}

GLuint Game::getProgramHandle() {
	GLuint pHandle = glCreateProgram();
	if (0 == pHandle)
	{
		Error::showError("Error creating program object.\n", true);
		return NULL;
	} else {
		this->programHandle = pHandle;
		return pHandle;
	}
}



void Game::linkProgram() {
	glLinkProgram(this->programHandle);

	GLint status;
	glGetProgramiv(this->programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		Error::showError("Failed to link shader program!\n", false);
		GLint logLen;
		glGetProgramiv(this->programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(this->programHandle, logLen, &written, log);
			Error::showError("Program log: \n%s", true);
			free(log);
		}
	}
	else
	{
		glUseProgram(this->programHandle);
	}
}