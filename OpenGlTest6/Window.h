#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"

using namespace std;

typedef void(*callback_function)(void); // type for conciseness

class Window {
	private:
		GLFWwindow* window;
		int swapInterval = 1;
		void init();
		void select();
		int width;
		int height;
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	public:
		Window(int width, int height, char* title);
		void render(callback_function renderFunc);
		void destroy();
};

Window::Window(int width, int height, char* title)
{
	this->width = width;
	this->height = height;
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	init();
	select();
}

void Window::init() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(swapInterval);
	glfwSetErrorCallback(Error::errorCallback);
	glfwSetKeyCallback(window, Window::keyCallback);
}

void Window::render(callback_function renderFunc) {

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, this->width, this->height);
		glClear(GL_COLOR_BUFFER_BIT);
		renderFunc();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Window::select() {
	glfwMakeContextCurrent(window);
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