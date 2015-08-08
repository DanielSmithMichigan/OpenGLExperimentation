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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate and bind the texture for normals
	glGenTextures(1, &normalsTexture);
	glBindTexture(GL_TEXTURE_2D, normalsTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Bind the FBO so that the next operations will be bound to it.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	// Attach the texture to the FBO
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseTexture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, positionTexture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalsTexture, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		Error::showError("Could not create fbo\n", true);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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
	GLenum DrawBuffers[] = { 
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 
	};
	glDrawBuffers(3, DrawBuffers);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
}

void Defer::end() {
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Defer::Update() {

}

void Defer::Draw(GlobalGameObjects* objects) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

	glBindSampler(0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, positionTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normalsTexture);

	*diffuseUniform << 0;
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