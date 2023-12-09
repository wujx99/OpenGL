#pragma once
#include <glad/glad.h>
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef DEBUG
#define GLCallV( x ) \
		 GLClearErrors(); \
		 x; \
		 if ( !GLCheckErrors( #x, __FILE__, __LINE__) ) __debugbreak();
#define GLCall( x ) [&]() { \
		 GLClearErrors(); \
		 auto retVal = x; \
		 if ( !GLCheckErrors( #x, __FILE__, __LINE__) ) __debugbreak(); \
		 return retVal; \
	   }()
#else
#define GLCallV( x ) x;
#define GLCall( x ) x;
#endif
void GLClearErrors();


bool GLCheckErrors(const char* func, const char* file, unsigned int line);

class Renderer
{
public:
	/*
	what actually we need? VertexArray(must have contain VertexBuffer,so wo can refactor the VertexArray class), Index buffer,
	and Material(shader and data like uniform, now we just use shader,and set Uniform manually!)
	*/
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

	void Clear();
};