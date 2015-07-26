#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "DrawableGameComponent.h"
#include "Colors.h"
#include <vector>
#include "Defer.h"

using namespace std;

class Window {
	private:
		GLFWwindow* window;
		int swapInterval = 1;
		void init();
		void select();
		int width;
		int height;
		char* title;
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void setSwapInterval();
		void handleKeypress();
		void disableMouse();
		void handleMouseMovement();
		int keysToCheck[6];
		GlobalGameObjects *objects;
	public:
		Window(int width, int height, char* title, GlobalGameObjects *objects);
		void render(Defer *deferred, vector<Component*> components);
		void destroy();
};

Window::Window(int width, int height, char* title, GlobalGameObjects *objects)
	: objects(objects)
{
	keysToCheck[0] = GLFW_KEY_W;
	keysToCheck[1] = GLFW_KEY_A;
	keysToCheck[2] = GLFW_KEY_S;
	keysToCheck[3] = GLFW_KEY_D;
	keysToCheck[4] = GLFW_KEY_UP;
	keysToCheck[5] = GLFW_KEY_DOWN;
	this->width = width;
	this->height = height;
	this->title = title;
	init();
	disableMouse();
	select();
	setSwapInterval();
}

void Window::init() {
	glfwSetErrorCallback(Error::errorCallback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, Window::keyCallback);	
}

void Window::disableMouse(){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0.0, 0.0);
};

void Window::handleMouseMovement() {
	double offsetX, offsetY;
	glfwGetCursorPos(window, &offsetX, &offsetY);
	objects->camera->handleMouseMovement(offsetX, offsetY);
	glfwSetCursorPos(window, 0.0, 0.0);
}

void Window::render(Defer *deferred, vector<Component*> components) {
	static const GLfloat one = 1.0f;
	double lastTime = 0;
	int numFrames = 0;
	double currTime = 0;

	while (!glfwWindowShouldClose(window))
	{
		numFrames++;
		if (glfwGetTime() - lastTime > 1.0) {
			cout << numFrames << endl;
			numFrames = 0;
			lastTime += 1.0;
		}
		handleKeypress();
		handleMouseMovement();
		glViewport(0, 0, this->width, this->height);
		deferred->start();
		for (Component* component : components)
		{
			DrawableGameComponent* drawableGameComponent = dynamic_cast<DrawableGameComponent*>(component);
			if (drawableGameComponent != nullptr) {
				drawableGameComponent->doDraw(objects);
			}
		}
		deferred->end();
		deferred->doDraw(objects);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Window::select() {
	glfwMakeContextCurrent(window);
}

void Window::setSwapInterval() {
	glfwSwapInterval(this->swapInterval);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Window::handleKeypress() {
	for (int i = 0; i < 6; i++) {
		if (glfwGetKey(window, keysToCheck[i]) == 1) {
			objects->camera->handleKeypress(keysToCheck[i]);
		}
	}
}

void Window::destroy() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}