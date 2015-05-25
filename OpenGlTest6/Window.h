#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "Component.h"

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
	public:
		Window(int width, int height, char* title);
		void render(Component* components[], int length);
		void destroy();
};

Window::Window(int width, int height, char* title)
{
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

void Window::render(Component* components[], int length) {
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, this->width, this->height);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < length; i++) {
			components[i]->Draw();
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

void Window::destroy() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}
