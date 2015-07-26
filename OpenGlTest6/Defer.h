#ifndef DEFER_HEADER
#define DEFER_HEADER
#include "Error.h"
#include "DrawableGameComponent.h"

class Defer : public DrawableGameComponent {
	public:
		Defer(int width, int height);
		void start();
		void end();
		void Initialize();
		void InitializeUniforms();
		void Update();
		void Draw(GlobalGameObjects* objects);
	private:
		char* fragShaderName;
		char* vertShaderName;
		int width;
		int height;
		GLuint fbo;
		GLuint diffuseTexture;
		GLuint positionTexture;
		GLuint normalsTexture;
		GLuint depthBuffer;
		GLuint blendTexture;
		OGLVariable *diffuseUniform;
		OGLVariable *positionUniform;
		OGLVariable *normalsUniform;
		OGLVariable* ambientColorUniform;
		OGLVariable* lightColorUniform;
		OGLVariable* lightDirectionUniform;
		OGLVariable* cameraPositionUniform;
		OGLVariable* specularColorUniform;
		OGLVariable* specularPowerUniform;
};

Defer::Defer(int width, int height)
	: width(width)
	, height(height)
	, DrawableGameComponent("deferredRendering.vert", "deferredRendering.frag") {
}

void Defer::Initialize()
{

	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &depthBuffer);

	// Bind the depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

	// Generate and bind the texture for diffuse
	glGenTextures(1, &diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate and bind the texture for positions
	glGenTextures(1, &positionTexture);
	glBindTexture(GL_TEXTURE_2D, positionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate and bind the texture for normals
	glGenTextures(1, &normalsTexture);
	glBindTexture(GL_TEXTURE_2D, normalsTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT,	NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate and bind the texture for blending data
	glGenTextures(1, &blendTexture);
	glBindTexture(GL_TEXTURE_2D, blendTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,	GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Bind the FBO so that the next operations will be bound to it.
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Attach the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, positionTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalsTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, diffuseTexture, 0);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		Error::showError("Could not create fbo\n", true);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Defer::InitializeUniforms() {
	diffuseUniform = new OGLVariable("DiffuseTexture", programHandle);
	positionUniform = new OGLVariable("PositionTexture", programHandle);
	normalsUniform = new OGLVariable("NormalsTexture", programHandle);
	ambientColorUniform = new OGLVariable("AmbientColor", programHandle);
	lightColorUniform = new OGLVariable("LightColor", programHandle);
	lightDirectionUniform = new OGLVariable("LightDirection", programHandle);
	cameraPositionUniform = new OGLVariable("CameraPosition", programHandle);
	specularColorUniform = new OGLVariable("SpecularColor", programHandle);
	specularPowerUniform = new OGLVariable("SpecularPower", programHandle);
}

void Defer::start() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	GLenum windowBuffClear[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, windowBuffClear);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLenum windowBuffOpaque[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, windowBuffOpaque);
}

void Defer::end() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Defer::Update() {

}

void Defer::Draw(GlobalGameObjects* objects) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normalsTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, positionTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	
	*diffuseUniform << 3;
	*positionUniform << 1;
	*normalsUniform << 2;
	*ambientColorUniform << objects->ambientLight->color;
	*lightColorUniform << objects->sun->color;
	*lightDirectionUniform << objects->sun->direction;
	*cameraPositionUniform << objects->camera->position;
	*specularColorUniform << objects->sun->color;
	*specularPowerUniform << 5.0f;
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f((float)width, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f((float)width, (float)height, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, (float)height, 0.0f);
	glEnd();
}

#endif