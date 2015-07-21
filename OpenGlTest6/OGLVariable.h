#include <string>
#include <GL/glew.h>
#include "Error.h"
#include <glm/glm.hpp>

class OGLVariable {
	public:
		OGLVariable(std::string name, GLuint programHandle);
		OGLVariable& operator<<(const glm::mat4 &value);
		OGLVariable& operator<<(const glm::mat3 &value);
		OGLVariable& operator<<(const glm::vec4 &value);
		OGLVariable& operator<<(const glm::vec3 &value);
		OGLVariable& operator<<(float value);
		std::string name;
		GLint location;
	private:
		GLuint programHandle;
};

OGLVariable::OGLVariable(std::string name, GLuint programHandle)
	: name(name),
	programHandle(programHandle) {
	location = glGetUniformLocation(programHandle, name.c_str());
	if (location == -1)
	{
		Error::showError("Cannot find Uniform", true);
	}
}

OGLVariable& OGLVariable::operator<<(const glm::mat4 &value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	return *this;
}

OGLVariable& OGLVariable::operator<<(const glm::mat3 &value)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	return *this;
}

OGLVariable& OGLVariable::operator<<(const glm::vec4 &value)
{
	glUniform4fv(location, 1, &value[0]);
	return *this;
}

OGLVariable& OGLVariable::operator<<(const glm::vec3& value)
{
	glUniform3fv(location, 1, &value[0]);
	return *this;
}

OGLVariable& OGLVariable::operator<<(float value)
{
	glUniform1f(location, value);
	return *this;
}