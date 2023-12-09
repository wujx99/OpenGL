#include "Renderer.h"
#include <iostream>

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

bool GLCheckErrors(const char* func, const char* file, unsigned int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "get error :" << error <<
			" " << func << " :" << file << ":" << line << std::endl;

		return false;
	}
	return true;
}