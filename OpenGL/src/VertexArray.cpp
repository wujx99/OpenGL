#include "VertexArray.h"
#include "ShaderDataType.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCallV(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCallV(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GLCallV(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCallV(glBindVertexArray(0));

}

void VertexArray::SetLayout(const BufferLayout& layout)
{
	m_Layout = layout;
	unsigned int index = 0;
	for (auto itr = m_Layout.Begin(); itr != m_Layout.End(); itr++)
	{
		GLCallV(glEnableVertexAttribArray(index));
		GLCallV(glVertexAttribPointer(index, itr->GetComponentCount(), ShaderDataTypeToOpenGLBaseType(itr->Type), 
			itr->Nomalized ? GL_TRUE : GL_FALSE, m_Layout.GetStride(), (const void*)itr->Offset));
		index++;
	}
}




