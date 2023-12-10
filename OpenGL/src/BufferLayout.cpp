#include "BufferLayout.h"



BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
	:m_elements(elements),m_stride(0)
{
	CalcStrideAndOffset();
}

void BufferLayout::CalcStrideAndOffset()
{
	m_stride = 0;
	unsigned int offset = 0;
	for (auto& element : m_elements) { //find a bug!!! add reference & to fix.
		element.Offset = offset;
		m_stride += element.Size;
		offset += element.Size;
	}
}
