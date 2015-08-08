#ifndef DRAWABLE_COMPONENT_HEADER
#define DRAWABLE_COMPONENT_HEADER
#include "Component.h"
#include "GlobalGameObjects.h"
#include "OGLVariable.h"

using namespace std;

class DrawableGameComponent : public Component {
	public:
		virtual void Update() = 0;
		void doDraw(GlobalGameObjects* objects);
		void doInitialize();
		DrawableGameComponent(char* fragShader, char* vertShader);
	private:
		virtual void Draw(GlobalGameObjects* objects) = 0;
		virtual void Initialize() = 0;
		virtual void InitializeUniforms() = 0;
		void linkProgram();
		void loadShaders();
		void createProgramHandle();
		void initDraw();
		void endDraw();
	protected:
		GLuint programHandle;
		char* fragShaderName;
		char* vertShaderName;
};

DrawableGameComponent::DrawableGameComponent(char* vertShaderName = "componentShader.vert", char* fragShaderName = "componentShader.frag")
: fragShaderName(fragShaderName)
, vertShaderName(vertShaderName)
{
}

void DrawableGameComponent::doInitialize() {
	createProgramHandle();
	Initialize();
	loadShaders();
	linkProgram();
	glUseProgram(programHandle);
	InitializeUniforms();
	glUseProgram(0);
}

void DrawableGameComponent::initDraw() {
	glUseProgram(programHandle);
}

void DrawableGameComponent::doDraw(GlobalGameObjects* objects) {
	initDraw();
	Draw(objects);
	endDraw();
}

void DrawableGameComponent::endDraw() {
	glUseProgram(0);
}

void DrawableGameComponent::createProgramHandle() {
	programHandle = glCreateProgram();
	if (0 == programHandle)
	{
		Error::showError("Error creating program object.\n", true);
	}
}

void DrawableGameComponent::loadShaders() {
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, vertShaderName, programHandle);
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, fragShaderName, programHandle);
}

void DrawableGameComponent::linkProgram() {
	glLinkProgram(programHandle);
	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		Error::showError("Failed to link shader program!\n", false);
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			Error::showError("Program log: \n%s", true);
			free(log);
		}
	}
}

#endif