#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count)
{
	GLCallV(glGenBuffers(1, &m_RendererID));
	GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCallV(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW))
}

IndexBuffer::~IndexBuffer()
{

	GLCallV(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

}

void IndexBuffer::UnBind() const
{
	GLCallV(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}
