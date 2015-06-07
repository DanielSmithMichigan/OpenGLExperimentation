#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "Component.h"
#include "Colors.h"
#include <vector>
#include "Camera.h"

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
		int keysToCheck[6];
		Camera* camera;
	public:
		Window(int width, int height, char* title);
		void render(vector<Component*> components);
		void destroy();
};

Window::Window(int width, int height, char* title) : camera(new Camera)
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
	select();
	setSwapInterval();
}

void Window::init() {
	glfwSetErrorCallback(Error::errorCallback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	this->window = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
	if (!this->window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, Window::keyCallback);	
}

void Window::render(vector<Component*> components) {
	static const GLfloat one = 1.0f;

	while (!glfwWindowShouldClose(window))
	{
		handleKeypress();
		glViewport(0, 0, this->width, this->height);
		glClearBufferfv(GL_COLOR, 0, &Colors::Black[0]);
		glClearBufferfv(GL_DEPTH, 0, &one);
		for (Component* component : components)
		{
			component->Draw(camera);
		}
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
			camera->handleKeypress(keysToCheck[i]);
		}
	}
}

void Window::destroy() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}