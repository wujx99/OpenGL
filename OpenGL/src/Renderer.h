#pragma once
#include <glad/glad.h>

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
