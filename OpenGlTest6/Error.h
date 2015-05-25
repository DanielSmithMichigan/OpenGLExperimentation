#ifndef ERROR_HEADER
#define ERROR_HEADER

#include <fstream>

class Error {
	public:
		static void errorCallback(int error, const char* description);
		static void showError(const char* description, bool endProgram);
		static void showError(const char* description);
	private:
};

void errorCallback(int error, const char* description)
{
	Error::showError(description);
}

void showError(const char* description) {
	Error::showError(description, true);
}

void showError(const char* description, bool endProgram) {
	fputs(description, stderr);
	if (endProgram) {
		exit(1);
	}
}

#endif