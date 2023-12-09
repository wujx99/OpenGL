#pragma once
#include <cstdint>
#include <Renderer.h>
#include <iostream>
#include <vector>

#include "ShaderDataType.h"
struct BufferElement
{
	std::string Name;
	ShaderDataType Type;
	uint32_t Size;
	uint32_t Offset;
	bool Nomalized;
public:
	BufferElement() {};
	BufferElement(const std::string& name, ShaderDataType type, bool nomalized = false)
		:Name(name), Type(type), Size(ShaderDataTypeSize(Type)), Offset(0),Nomalized(nomalized) {
		
	}
	uint32_t GetComponentCount() {
		switch (Type)
		{
			case ShaderDataType::Float:    return 1;
			case ShaderDataType::Float2:   return 2;
			case ShaderDataType::Float3:   return 3;
			case ShaderDataType::Float4:   return 4;
			case ShaderDataType::Mat3:     return 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4;
			case ShaderDataType::Int:      return 1;
			case ShaderDataType::Int2:     return 2;
			case ShaderDataType::Int3:     return 3;
			case ShaderDataType::Int4:     return 4;
			case ShaderDataType::Bool:     return 1;
		}

		std::cout << "Unknown ShaderDataType!" << std::endl;
		return 0;
	}
};
class BufferLayout
{
private:
	std::vector<BufferElement> m_elements;
	uint32_t m_stride;
public:
	BufferLayout();
	BufferLayout(std::initializer_list<BufferElement> elements);

	inline uint32_t GetStride() const { return m_stride; };
	inline const std::vector<BufferElement>& GetElements() { return m_elements; };

	std::vector<BufferElement>::iterator Begin() { return m_elements.begin(); };
	std::vector<BufferElement>::iterator End() { return m_elements.end(); };
	std::vector<BufferElement>::const_iterator Begin() const { return m_elements.cbegin(); };
	std::vector<BufferElement>::const_iterator End() const{ return m_elements.cend(); };
private:
	void CalcStrideAndOffset();
};