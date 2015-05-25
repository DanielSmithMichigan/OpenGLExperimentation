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
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Component{
	public:
		void initialize(GLuint programHandle);
		void Draw();
		Component(GLuint programHandle);
	private:
};

Component::Component(GLuint programHandle) {
	this->initialize(programHandle);
}

void Component::initialize(GLuint programHandle) {
	// Bind index 0 to the shader input variable "VertexPosition"
	glBindAttribLocation(programHandle, 0, "VertexPosition");

	// Bind index 1 to the shader input variable "VertexColor"
	glBindAttribLocation(programHandle, 1, "VertexColor");
}

void Component::Draw() {
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