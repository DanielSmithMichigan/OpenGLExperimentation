#ifndef ERROR_HEADER
#define ERROR_HEADER

#include <fstream>

class Error {
	public:
		static void errorCallback(int error, const char* description);
		static void showError(const char* description, bool endProgram);
		static void showError(const char* description);
		static void checkError();
	private:
};

void Error::errorCallback(int error, const char* description)
{
	Error::showError(description);
}

void Error::showError(const char* description) {
	Error::showError(description, true);
}

void Error::showError(const char* description, bool endProgram) {
	fputs(description, stderr);
	if (endProgram) {
		//exit(1);
	}
}

void Error::checkError() {
	GLenum err = glGetError();

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		Error::showError(error.c_str());
		err = glGetError();
	}
}

#endif