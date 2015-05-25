#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include "Error.h"
#ifndef SHADER_HEADER
#define SHADER_HEADER
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Shader {
private:
		char* loadShaderContents();
		char* fileName;
		GLenum shaderType;
		GLuint shader;
		GLuint init(GLenum shaderType);
		GLuint load(GLuint programHandle);
	public:
		Shader(GLenum shaderType, char* fileName, GLuint programHandle);
};

Shader::Shader(GLenum shaderType, char* fileName, GLuint programHandle)
{
	this->shaderType = shaderType;
	this->fileName = fileName;
	load(programHandle);
}

GLuint Shader::init(GLenum shaderType) 
{
	GLuint shader = glCreateShader(shaderType);
	if (0 == shader)
	{
		Error::showError("Error creating vertex shader.\n", true);
		return NULL;
	}
	else {
		this->shader = shader;
		return shader;
	}
}

GLuint Shader::load(GLuint programHandle)
{
	const GLchar* shaderCode = loadShaderContents();
	const GLchar* codeArray[] = { shaderCode };
	glShaderSource(this->shader, 1, codeArray, NULL);
	glCompileShader(this->shader);
	GLint result;
	glGetShaderiv(this->shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		Error::showError("Vertex shader compilation failed!\n", false);
		GLint logLen;
		glGetShaderiv(this->shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			Error::showError("Shader log:\n%s", false);
			Error::showError(log, true);
			free(log);
		}
	}
	glAttachShader(programHandle, shader);
	return shader;
}

char* Shader::loadShaderContents()
{
	ifstream in(this->fileName);
	string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
	char * cstr = new char[contents.length() + 1];
	strcpy_s(cstr, contents.length() + 1, contents.c_str());
	return cstr;
}

#endif