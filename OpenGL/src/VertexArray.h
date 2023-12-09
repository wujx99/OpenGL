#pragma once
#include "BufferLayout.h"
class VertexArray
{
private:
	unsigned int m_RendererID;
	BufferLayout m_Layout;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void UnBind() const;

	void SetLayout(const BufferLayout& layout);
	const BufferLayout& GetLayout() const { return m_Layout; };
};