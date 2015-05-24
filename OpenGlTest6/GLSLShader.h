#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

namespace GLSLShader {
	enum GLSLShaderType {
		VERTEX, FRAGMENT, GEOMETRY, TESS_CONTROL, TESS_EVALUATION
	};
};

class GLSLProgram {
	private:
		int handle;
		bool linked;
		string logString;
		int getUniformLocation(const char* name);
		bool fileExists(const string& fileName);
	public:
		GLSLProgram();
		bool compileShaderFromFile(const char* fileName, GLSLShader::GLSLShaderType type);
		bool compileShaderFromString(const string& source, GLSLShader::GLSLShaderType type);
		bool link();
		void use();
		std::string log();
		int getHandle();
		bool isLinked();
		void bindAttribLocation(GLuint location, const char * name);
		void bindFragDataLocation(GLuint location, const char * name);
		void setUniform(const char *name, float x, float y, float z);
		void setUniform(const char *name, const vec3 & v);
		void setUniform(const char *name, const vec4 & v);
		void setUniform(const char *name, const mat4 & m);
		void setUniform(const char *name, const mat3 & m);
		void setUniform(const char *name, float val);
		void setUniform(const char *name, int val);
		void setUniform(const char *name, bool val);
		void printActiveUniforms();
		void printActiveAttribs();
};

int GLSLProgram :: getUniformLocation(const char* name) {

}