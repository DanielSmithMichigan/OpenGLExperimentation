#ifndef PROGRAM_HANDLE
#define PROGRAM_HANDLE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class ProgramHandle {
	private:
		static GLuint programHandle;
	public:
		static void setProgramHandle(GLuint handle);
		static GLuint getProgramHandle();
};
GLuint ProgramHandle::programHandle;

void ProgramHandle::setProgramHandle(GLuint handle) {
	programHandle = handle;
}

GLuint ProgramHandle::getProgramHandle() {
	return programHandle;
}

#endif