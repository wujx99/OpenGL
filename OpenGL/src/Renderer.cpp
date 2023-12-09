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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.Bind();
	ib.Bind();
	va.Bind();
	GLCallV(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear()
{
	GLCallV(glClear(GL_COLOR_BUFFER_BIT));

}
