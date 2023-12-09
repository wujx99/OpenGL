#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();

	inline unsigned int GetCount() { return m_Count; }
	void Bind() const;
	void UnBind() const;
};