#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>

int WINDOW_HEIGHT = 600;
int WINDOW_WIDTH = 800;
char* WINDOW_TITLE = "Ya did it harry";

using namespace std;



void drawTriangle(float positionData[], float colorData[])
{
	GLuint vaoHandle;

	// Create the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	// Populate the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData,
		GL_STATIC_DRAW);

	// Populate the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData,
		GL_STATIC_DRAW);

	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // Vertex position
	glEnableVertexAttribArray(1); // Vertex color

	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

char* loadShaderAsString(string shaderFileName) {
	ifstream in(shaderFileName);
	string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
	char * cstr = new char[contents.length() + 1];
	strcpy_s(cstr, contents.length() + 1, contents.c_str());
	return cstr;
}

GLuint loadShader(GLenum shaderType, string fileName, GLuint programHandle)
{
	GLuint shader = glCreateShader(shaderType);
	if (0 == shader)
	{
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(1);
	}
	const GLchar* shaderCode = loadShaderAsString(fileName);
	const GLchar* codeArray[] = { shaderCode };
	glShaderSource(shader, 1, codeArray, NULL);
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		fprintf(stderr, "Vertex shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			fprintf(stderr, "Shader log:\n%s", log);
			free(log);
		}
	}
	glAttachShader(programHandle, shader);
	return shader;
}

GLuint getProgramHandle() {
	GLuint programHandle = glCreateProgram();
	if (0 == programHandle)
	{
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}
	else {
		return programHandle;
	}
}

void linkProgram(GLuint programHandle) {
	glLinkProgram(programHandle);

	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		fprintf(stderr, "Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
				&written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
	}
	else
	{
		glUseProgram(programHandle);
	}

}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);

	glewInit();

	GLuint programHandle = getProgramHandle();

	GLuint vertShader = loadShader(GL_VERTEX_SHADER, "shader.vert", programHandle);
	GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, "shader.frag", programHandle);

	// Bind index 0 to the shader input variable "VertexPosition"
	glBindAttribLocation(programHandle, 0, "VertexPosition");

	// Bind index 1 to the shader input variable "VertexColor"
	glBindAttribLocation(programHandle, 1, "VertexColor");

	linkProgram(programHandle);

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glClear(GL_COLOR_BUFFER_BIT);



		float positionData[] = {
			-0.8f, -0.8f, 0.0f,
			0.8f, -0.8f, 0.0f,
			0.0f, 0.8f, 0.0f
		};

		float colorData[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		drawTriangle(positionData, colorData);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}